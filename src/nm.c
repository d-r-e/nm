#include <nm.h>

void close_file(int fd, void* ptr, struct stat* statbuf) {
  if (ptr && ptr != MAP_FAILED)
    munmap(ptr, statbuf->st_size);
  if (fd > 0)
    close(fd);
}

void _print_ehdr(Elf64_Ehdr* ehdr) {
  printf("e_ident: %s\n", ehdr->e_ident);
  printf("e_type: %d\n", ehdr->e_type);
  printf("e_machine: %d\n", ehdr->e_machine);
  printf("e_version: %d\n", ehdr->e_version);
  printf("e_entry: %p\n", (void*)ehdr->e_entry);
  printf("e_phoff: %ld\n", ehdr->e_phoff);
  printf("e_shoff: %ld\n", ehdr->e_shoff);
  printf("e_flags: %d\n", ehdr->e_flags);
  printf("e_ehsize: %d\n", ehdr->e_ehsize);
  printf("e_phentsize: %d\n", ehdr->e_phentsize);
  printf("e_phnum: %d\n", ehdr->e_phnum);
  printf("e_shentsize: %d\n", ehdr->e_shentsize);
  printf("e_shnum: %d\n", ehdr->e_shnum);
  printf("e_shstrndx: %d\n", ehdr->e_shstrndx);
}

void _file_format_no_recognized(char* filename, int fd, void* ptr, struct stat* statbuf) {
  fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
          filename);
  close_file(fd, ptr, statbuf);
}

bool is_debug(Elf64_Sym sym) {
  return sym.st_shndx == SHN_UNDEF && ELF64_ST_BIND(sym.st_info) == STB_LOCAL;
}

static t_symbol* sort(t_symbol* symbols, int flags) {
  t_symbol* sorted = NULL;
  t_symbol* tmp = NULL;
  t_symbol* prev = NULL;
  t_symbol* next = NULL;

  while (symbols) {
    next = symbols->next;
    if (sorted == NULL) {
      sorted = symbols;
      sorted->next = NULL;
    } else {
      tmp = sorted;
      prev = NULL;
      while (tmp) {
        int str_cmp;
        if (flags & FLAG_R)
          str_cmp = -ft_strcmp(symbols->name, tmp->name);
        else
          str_cmp = ft_strcmp(symbols->name, tmp->name);
        if (str_cmp < 0 || str_cmp == 0) {
          if (str_cmp == 0 && (flags & FLAG_R)) {
            if (symbols->sym->st_value > tmp->sym->st_value)
              break;
          } else if (str_cmp == 0 && !(flags & FLAG_R)) {
            if (symbols->sym->st_value < tmp->sym->st_value) {
              break;
            }
          } else if (str_cmp < 0) {
            break;
          }
          break;
        }
        prev = tmp;
        tmp = tmp->next;
      }
      if (prev == NULL) {
        symbols->next = sorted;
        sorted = symbols;
      } else {
        symbols->next = prev->next;
        prev->next = symbols;
      }
    }
    symbols = next;
  }
  return sorted;
}

static void _nm64(void* ptr, int flags, struct stat* statbuff, char* filename) {
  (void)flags;
  bool symtab_found = false;
  Elf64_Ehdr* ehdr = (Elf64_Ehdr*)ptr;
  Elf64_Shdr* shdr;
  Elf64_Shdr* shstrtab;
  char* shstrtab_p;

  if (ehdr->e_shoff + sizeof(Elf64_Shdr) * ehdr->e_shnum >
      (size_t)statbuff->st_size)
    _file_format_no_recognized(filename, -1, ptr, statbuff);
  shdr = (Elf64_Shdr*)(ptr + ehdr->e_shoff);
  shstrtab = &shdr[ehdr->e_shstrndx];
  if (shstrtab->sh_offset + shstrtab->sh_size > (size_t)statbuff->st_size)
    _file_format_no_recognized(filename, -1, ptr, statbuff);

  shstrtab_p = (char*)(ptr + shstrtab->sh_offset);

  if (ehdr->e_shnum == 0 || ehdr->e_shstrndx == SHN_UNDEF)
    _file_format_no_recognized(filename, -1, ptr, statbuff);

  for (unsigned int i = 0; i < ehdr->e_shnum; i++) {
    if (shdr[i].sh_name >= shstrtab->sh_size)
      _file_format_no_recognized(filename, -1, ptr, statbuff);
    if (ft_strcmp(".symtab", shstrtab_p + shdr[i].sh_name) == 0) {
      symtab_found = true;
      if (shdr[i].sh_offset + shdr[i].sh_size > (size_t)statbuff->st_size ||
          shdr[shdr[i].sh_link].sh_offset > (size_t)statbuff->st_size) {
        fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
                filename);
        close_file(-1, ptr, statbuff);
      }

      Elf64_Sym* symtab = (Elf64_Sym*)(ptr + shdr[i].sh_offset);
      char* strtab = (char*)(ptr + shdr[shdr[i].sh_link].sh_offset);

      for (unsigned int j = 0; j < shdr[i].sh_size / sizeof(Elf64_Sym); j++) {
        if ((size_t)(strtab + symtab[j].st_name) >=
            (size_t)(ptr + statbuff->st_size)) {
          fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
                  filename);
          close_file(-1, ptr, statbuff);
        }
      }
    }
  }
  if (!symtab_found) {
    fprintf(stderr, "%s: %s: no symbols\n", PROGRAM_NAME, filename);
    close_file(-1, ptr, statbuff);
  }
  for (unsigned int i = 0; i < ehdr->e_shnum; i++) {
    if (ft_strcmp(".symtab", shstrtab_p + shdr[i].sh_name) == 0) {
      Elf64_Sym* symtab = (Elf64_Sym*)(ptr + shdr[i].sh_offset);
      char* strtab = (char*)(ptr + shdr[shdr[i].sh_link].sh_offset);
      t_symbol* symbols = NULL;

      for (unsigned int j = 0; j < shdr[i].sh_size / sizeof(Elf64_Sym); j++) {
        if (symtab[j].st_name != 0) {
          t_symbol* new_symbol = malloc(sizeof(t_symbol));
          new_symbol->sym = &symtab[j];
          new_symbol->name = strtab + symtab[j].st_name;
          new_symbol->type = _get_symbol_char(symtab[j], shdr);
          new_symbol->value = ft_itoa(symtab[j].st_value);
          new_symbol->shndx = ft_itoa(symtab[j].st_shndx);
          new_symbol->next = symbols;
          symbols = new_symbol;
        }
      }
      symbols = sort(symbols, flags);

      while (symbols) {
        if (!flags) {
          if (is_debug(*symbols->sym) || ft_strchr("a", symbols->type)) {
            symbols = symbols->next;
            continue;
          }

          //   print_type_bind_shn(&shdr[symbols->sym->st_shndx],
          //   ELF64_ST_TYPE(symbols->sym->st_info),
          //   ELF64_ST_BIND(symbols->sym->st_info));
          if (!ft_strchr("Uw", symbols->type)) {
            printf("%016lx %c %s\n",
                   symbols->sym
                       ->st_value,  // + shdr[symbols->sym->st_shndx].sh_addr,
                   symbols->type, symbols->name);
            (void)print_Elf64_Shdr;
            // if (!ft_strcmp(symbols->name, "status"))
            //     print_Elf64_Shdr(&shdr[symbols->sym->st_shndx]);
          } else
            printf("%16c %c %s\n", ' ', symbols->type, symbols->name);
        }
        free(symbols->shndx);
        free(symbols->value);
        symbols = symbols->next;
      }
    }
  }
}

// https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-35342/index.html
void nm(char* filename, int flags) {
  int fd;
  struct stat statbuf;
  void* ptr;

  (void)_print_ehdr;
  if ((fd = open(filename, O_RDONLY)) < 0) {
    fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, filename, strerror(errno));
  }
  if (fstat(fd, &statbuf) < 0) {
    fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, filename, strerror(errno));
    close_file(fd, NULL, &statbuf);
  }
  if ((unsigned int)statbuf.st_size < (sizeof(Elf64_Ehdr))) {
    fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
            filename);
    close_file(fd, NULL, &statbuf);
  }
  if ((ptr = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
      MAP_FAILED) {
    perror("mmap");
    close_file(fd, NULL, &statbuf);
  }
  if (ft_memcmp(ELFMAG, ptr, SELFMAG) != 0) {
    fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
            filename);
    close_file(fd, ptr, &statbuf);
  }
  Elf64_Ehdr* ehdr = (Elf64_Ehdr*)ptr;
  if ((ehdr->e_ident[EI_CLASS] != ELFCLASS64 &&
       ehdr->e_ident[EI_CLASS] != ELFCLASS32) ||
      ehdr->e_ident[EI_VERSION] != EV_CURRENT) {
    fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
            filename);
    close_file(fd, ptr, &statbuf);
  }
  if (ehdr->e_ident[EI_CLASS] == ELFCLASS64) {
    _nm64(ptr, flags, &statbuf, filename);
  } else {
    fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
            filename);
  }
  close_file(fd, ptr, &statbuf);
}

int main(int argc, char** argv) {
  int flags = 0;
  int opt;
  int optind = 1;
  int file_count;

  while ((opt = ft_getopt(argc, argv, "purga", &optind)) != -1) {
    switch (opt) {
      case 'a':
        flags |= FLAG_A;
        break;
      case 'r':
        flags |= FLAG_R;
        break;
      case 'p':
        flags |= FLAG_P;
        break;
      case 'u':
        flags |= FLAG_U;
        break;
      default:
        fprintf(stderr, "Usage: %s [-flags] [file]\n", PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }
  }
  file_count = argc - optind;
  if (file_count == 0) {
    nm("a.out", flags);
  } else {
    for (; optind < argc; optind++) {
      if (file_count > 1){
        write(1, argv[optind], ft_strlen(argv[optind]));
        ft_puts(":");
      }
      nm(argv[optind], flags);
    }
  }
}