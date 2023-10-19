#include <nm.h>

static void _print_ehdr(Elf64_Ehdr* ehdr) {
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

void _exit_cleanup(int fd, void* ptr, struct stat* statbuf, int exit_code) {
  if (ptr && ptr != MAP_FAILED)
    munmap(ptr, statbuf->st_size);
  if (fd > 0)
    close(fd);
  exit(exit_code);
}

void print_type_bind_shn(Elf64_Shdr* shdr,
                         unsigned char type,
                         unsigned char bind) {
  // Prints, for example, "OBJECT GLOBAL DEFAULT 2"
  char* type_str;
  char* bind_str;
  char* shn_str;

  switch (type) {
    case STT_NOTYPE:
      type_str = "NOTYPE";
      break;
    case STT_OBJECT:
      type_str = "OBJECT";
      break;
    case STT_FUNC:
      type_str = "FUNC";
      break;
    case STT_SECTION:
      type_str = "SECTION";
      break;
    case STT_FILE:
      type_str = "FILE";
      break;
    case STT_COMMON:
      type_str = "COMMON";
      break;
    case STT_TLS:
      type_str = "TLS";
      break;
    case STT_NUM:
      type_str = "NUM";
      break;
    case STT_LOOS:
      type_str = "LOOS";
      break;
    case STT_HIOS:
      type_str = "HIOS";
      break;
    case STT_LOPROC:
      type_str = "LOPROC";
      break;
    case STT_HIPROC:
      type_str = "HIPROC";
      break;
    default:
      type_str = "UNKNOWN";
      break;
  }

  switch (bind) {
    case STB_LOCAL:
      bind_str = "LOCAL";
      break;
    case STB_GLOBAL:
      bind_str = "GLOBAL";
      break;
    case STB_WEAK:
      bind_str = "WEAK";
      break;
    case STB_NUM:
      bind_str = "NUM";
      break;
    case STB_LOOS:
      bind_str = "LOOS";
      break;
    case STB_HIOS:
      bind_str = "HIOS";
      break;
    case STB_LOPROC:
      bind_str = "LOPROC";
      break;
    case STB_HIPROC:
      bind_str = "HIPROC";
      break;
    default:
      bind_str = "UNKNOWN";
      break;
  }

  if (shdr) {
    switch (shdr->sh_type) {
      case SHT_NULL:
        shn_str = "NULL";
        break;
      case SHT_PROGBITS:
        shn_str = "PROGBITS";
        break;
      case SHT_SYMTAB:
        shn_str = "SYMTAB";
        break;
      case SHT_STRTAB:
        shn_str = "STRTAB";
        break;
      case SHT_RELA:
        shn_str = "RELA";
        break;
      case SHT_HASH:
        shn_str = "HASH";
        break;
      case SHT_DYNAMIC:
        shn_str = "DYNAMIC";
        break;
      case SHT_NOTE:
        shn_str = "NOTE";
        break;
      case SHT_NOBITS:
        shn_str = "NOBITS";
        break;
      case SHT_REL:
        shn_str = "REL";
        break;
      case SHT_SHLIB:
        shn_str = "SHLIB";
        break;
      case SHT_DYNSYM:
        shn_str = "DYNSYM";
        break;
      case SHT_INIT_ARRAY:
        shn_str = "INIT_ARRAY";
        break;
      case SHT_FINI_ARRAY:
        shn_str = "FINI_ARRAY";
        break;
      case SHT_PREINIT_ARRAY:
        shn_str = "PREINIT_ARRAY";
        break;
      case SHT_GROUP:
        shn_str = "GROUP";
        break;
      case SHT_SYMTAB_SHNDX:
        shn_str = "SYMTAB_SHNDX";
        break;
      case SHT_NUM:
        shn_str = "NUM";
        break;
      case SHT_LOOS:
        shn_str = "LOOS";
        break;
      case SHT_GNU_ATTRIBUTES:
        shn_str = "GNU_ATTRIBUTES";
        break;
      case SHT_GNU_HASH:
        shn_str = "GNU_HASH";
        break;
      case SHT_GNU_LIBLIST:
        shn_str = "GNU_LIBLIST";
        break;
      case SHT_CHECKSUM:
        shn_str = "CHECKSUM";
        break;
      case SHT_LOSUNW:
        shn_str = "LOSUNW";
        break;
      case SHT_SUNW_COMDAT:
        shn_str = "SUNW_COMDAT";
        break;

      case SHT_SUNW_syminfo:
        shn_str = "SUNW_syminfo";
        break;
      default:
        shn_str = "UNKNOWN";
        break;
    }
  } else {
    shn_str = "UNKNOWN";
  }
  printf("%10s %14s %14s ", type_str, bind_str, shn_str);
}

char get_symbol_char(Elf64_Sym sym, Elf64_Shdr* shdr) {
  char c;
  unsigned char type = ELF64_ST_TYPE(sym.st_info);
  unsigned char bind = ELF64_ST_BIND(sym.st_info);

  switch (type) {
    case STT_NOTYPE:
      if (sym.st_shndx == SHN_UNDEF && bind == STB_GLOBAL)
        c = 'U';
      else if (bind == STB_LOCAL) {
        if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS) c = 'R';
        else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY) c = 'D';
        else c = '?';
      } else if (bind == STB_WEAK){
        if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS) c = 'W';
        else if (shdr[sym.st_shndx].sh_type == SHT_NULL) c = 'w';
      } 
      else if (bind == STB_GLOBAL &&
               (shdr[sym.st_shndx].sh_type == SHT_PROGBITS ||
                shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY))
        c = 'D';
      else if (bind == STB_GLOBAL && shdr[sym.st_shndx].sh_type == SHT_NOBITS)
        c = 'B';
      else
        c = '?';
      break;
    case STT_OBJECT:
      if (bind == STB_LOCAL && (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY ||
                                shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY ||
                                shdr[sym.st_shndx].sh_type == SHT_DYNAMIC))
        c = 'D';
      else if (sym.st_shndx == SHN_UNDEF)
        return 'U';
      else {
        switch (shdr[sym.st_shndx].sh_type) {
          case SHT_NOBITS:
            if (shdr[sym.st_shndx].sh_flags & SHF_WRITE)
              c = 'B';
            break;
          case SHT_PROGBITS:
            if (shdr[sym.st_shndx].sh_flags & SHF_WRITE)
              c = 'D';
            else
              c = 'R';
            break;
          default:
            c = '?';
            break;
        }
      }
      break;
    case STT_FUNC:
      if (bind == STB_WEAK && shdr[sym.st_shndx].sh_type == SHT_NULL)
        c = 'w';
      else if (bind != STB_GLOBAL && shdr[sym.st_shndx].sh_type == SHT_NULL &&
          !sym.st_shndx == SHN_UNDEF)
        c = 'w';
      else if (sym.st_shndx == SHN_UNDEF)
        return 'U';
      else
        c = 'T';
      break;
    case STT_SECTION:
      c = 'S';
      break;
    case STT_FILE:
      c = 'F';
      break;
    default:
      c = '?';
      break;
  }
    if (type == STT_OBJECT) {
      if (sym.st_shndx == SHN_UNDEF)
        c = 'W';

    } 
//       c = 'W';
//   }
  if (sym.st_shndx == SHN_ABS) {
    c = 'A';
  }
  // if weak and SHT_NULL, then uppercase W
  if (type == STT_NOTYPE && bind == STB_WEAK &&
      shdr[sym.st_shndx].sh_type == SHT_NULL) {
    c = ft_tolower(c);
  } else if ((bind == STB_LOCAL || (bind == STB_WEAK && c != 'W')) &&
             c != 'U' && c != 'V') {
    c = ft_tolower(c);
  }
  return c;
}

static
void print_Elf64_Shdr(Elf64_Shdr * shdr){
  printf("Elf64_Word  sh_name: %d\n", shdr->sh_name);
  printf("Elf64_Word  sh_type: %d\n", shdr->sh_type);
  printf("Elf64_Xword sh_flags: %ld\n", shdr->sh_flags);
  printf("Elf64_Addr  sh_addr: %p\n", (void*)shdr->sh_addr);
  printf("Elf64_Off   sh_offset: %ld\n", shdr->sh_offset);
  printf("Elf64_Xword sh_size: %ld\n", shdr->sh_size);
  printf("Elf64_Word  sh_link: %d\n", shdr->sh_link);
  printf("Elf64_Word  sh_info: %d\n", shdr->sh_info);
  printf("Elf64_Xword sh_addralign: %ld\n", shdr->sh_addralign);
  printf("Elf64_Xword sh_entsize: %ld\n", shdr->sh_entsize);
}

    void _file_format_no_recognized(char* filename,
                                    int fd,
                                    void* ptr,
                                    struct stat* statbuf) {
  fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
          filename);
  _exit_cleanup(fd, ptr, statbuf, EXIT_FAILURE);
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
        int nameComparison;
        if (flags & FLAG_R)
          nameComparison = -ft_strcmp(symbols->name, tmp->name);
       else 
          nameComparison = ft_strcmp(symbols->name, tmp->name);
        if (nameComparison < 0 ||
            (nameComparison == 0 &&
             symbols->sym->st_value < tmp->sym->st_value)) {
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
        _exit_cleanup(-1, ptr, statbuff, EXIT_FAILURE);
      }

      Elf64_Sym* symtab = (Elf64_Sym*)(ptr + shdr[i].sh_offset);
      char* strtab = (char*)(ptr + shdr[shdr[i].sh_link].sh_offset);

      for (unsigned int j = 0; j < shdr[i].sh_size / sizeof(Elf64_Sym); j++) {
        if ((size_t)(strtab + symtab[j].st_name) >=
            (size_t)(ptr + statbuff->st_size)) {
          fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
                  filename);
          _exit_cleanup(-1, ptr, statbuff, EXIT_FAILURE);
        }
      }
    }
  }
  if (!symtab_found) {
    fprintf(stderr, "%s: %s: no symbols\n", PROGRAM_NAME, filename);
    _exit_cleanup(-1, ptr, statbuff, EXIT_FAILURE);
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
          new_symbol->type = get_symbol_char(symtab[j], shdr);
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
          (void)print_Elf64_Shdr;
          if (!ft_strchr("Uw", symbols->type)){
            printf("%016lx %c %s\n",
                   symbols->sym->st_value, // + shdr[symbols->sym->st_shndx].sh_addr,
                   symbols->type, symbols->name);
                   (void)print_Elf64_Shdr;
            // if (!ft_strcmp(symbols->name, "status"))
            //     print_Elf64_Shdr(&shdr[symbols->sym->st_shndx]);
          }
          else
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
    exit(EXIT_FAILURE);
  }
  if (fstat(fd, &statbuf) < 0) {
    fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, filename, strerror(errno));
    _exit_cleanup(fd, NULL, &statbuf, EXIT_FAILURE);
  }
  if ((unsigned int)statbuf.st_size < (sizeof(Elf64_Ehdr))) {
    fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
            filename);
    _exit_cleanup(fd, NULL, &statbuf, EXIT_FAILURE);
  }
  if ((ptr = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
      MAP_FAILED) {
    perror("mmap");
    _exit_cleanup(fd, NULL, &statbuf, EXIT_FAILURE);
  }
  if (ft_memcmp(ELFMAG, ptr, SELFMAG) != 0) {
    fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
            filename);
    _exit_cleanup(fd, ptr, &statbuf, EXIT_FAILURE);
  }
  Elf64_Ehdr* ehdr = (Elf64_Ehdr*)ptr;
  if ((ehdr->e_ident[EI_CLASS] != ELFCLASS64 &&
       ehdr->e_ident[EI_CLASS] != ELFCLASS32) ||
      ehdr->e_ident[EI_VERSION] != EV_CURRENT) {
    fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
            filename);
    _exit_cleanup(fd, ptr, &statbuf, EXIT_FAILURE);
  }
  if (ehdr->e_ident[EI_CLASS] == ELFCLASS64) {
    _nm64(ptr, flags, &statbuf, filename);
  } else {
    fprintf(stderr, "%s: %s: File format not recognized\n", PROGRAM_NAME,
            filename);
    _exit_cleanup(fd, ptr, &statbuf, EXIT_FAILURE);
  }
  _exit_cleanup(fd, ptr, &statbuf, EXIT_SUCCESS);
}

static char* get_filename(int argc, char** argv, int* flags) {
  int opt;
  char* filename = "a.out";
  int optind = 1;

  while ((opt = ft_getopt(argc, argv, "purga", &optind)) != -1) {
    switch (opt) {
      case 'a':
        *flags |= FLAG_A;
        break;
      case 'r':
        *flags |= FLAG_R;
        break;
      case 'p':
        *flags |= FLAG_P;
        break;
      case 'u':
        *flags |= FLAG_U;
        break;
      default:
        fprintf(stderr, "Usage: %s [-flags] [file]\n", PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }
  }

  if (optind < argc)
    filename = argv[optind];

  return filename;
}
int main(int argc, char** argv) {
  int flags = 0;
  char* filename = get_filename(argc, argv, &flags);
  nm(filename, flags);
}