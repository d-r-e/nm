#include <nm.h>

static
void close_file(int fd, void* ptr, struct stat* statbuf, size_t size) {
	(void)statbuf;
	if (ptr && ptr != MAP_FAILED)
		munmap(ptr, size);
	if (fd > 0)
		close(fd);
}


static void _file_format_no_recognized(char* filename,
									   int fd,
									   void* ptr,
									   struct stat* statbuf) {
	fprintf(stderr, "%s: %s: file format not recognized\n", PROGRAM_NAME,
			filename);
	close_file(fd, ptr, statbuf, sizeof(Elf64_Ehdr));
}

static bool is_debug(Elf64_Sym sym) {
	return sym.st_shndx == SHN_UNDEF && ELF64_ST_BIND(sym.st_info) == STB_LOCAL;
}

static bool is_debug32(Elf32_Sym sym) {
	return sym.st_shndx == SHN_UNDEF && ELF32_ST_BIND(sym.st_info) == STB_LOCAL;
}

static void clear_symbol_list(t_symbol* symbols) {
	t_symbol* tmp;

	while (symbols) {
		tmp = symbols->next;
		free(symbols->shndx);
		free(symbols->value);
		free(symbols);
		symbols = tmp;
	}
}

static void _nm64(void* ptr, int flags, struct stat* statbuff, char* filename) {
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

		if (!ft_strcmp(".symtab", shstrtab_p + shdr[i].sh_name)) {
			symtab_found = true;
			if (shdr[i].sh_offset + shdr[i].sh_size >
					(size_t)statbuff->st_size ||
				shdr[shdr[i].sh_link].sh_offset > (size_t)statbuff->st_size) {
				fprintf(stderr, "%s: %s: File format not recognized\n",
						PROGRAM_NAME, filename);
				close_file(-1, ptr, statbuff, statbuff->st_size);
			}
			Elf64_Sym* symtab = (Elf64_Sym*)(ptr + shdr[i].sh_offset);
			char* strtab = (char*)(ptr + shdr[shdr[i].sh_link].sh_offset);

			for (unsigned int j = 0; j < shdr[i].sh_size / sizeof(Elf64_Sym);
				 j++) {
				if ((size_t)(strtab + symtab[j].st_name) >=
					(size_t)(ptr + statbuff->st_size)) {
					fprintf(stderr, "%s: %s: File format not recognized\n",
							PROGRAM_NAME, filename);
					close_file(-1, ptr, statbuff, statbuff->st_size);
				}
			}
		}
	}
	if (!symtab_found) {
		fprintf(stderr, "%s: %s: no symbols\n", PROGRAM_NAME, filename);
		close_file(-1, ptr, statbuff, statbuff->st_size);
		return;
	}
	for (unsigned int i = 0; i < ehdr->e_shnum; i++) {
		if (ft_strcmp(".symtab", shstrtab_p + shdr[i].sh_name) == 0) {
			Elf64_Sym* symtab = (Elf64_Sym*)(ptr + shdr[i].sh_offset);
			char* strtab = (char*)(ptr + shdr[shdr[i].sh_link].sh_offset);
			t_symbol* symbols = NULL;
			for (unsigned int j = 0; j < shdr[i].sh_size / sizeof(Elf64_Sym);
				 j++) {
				if (symtab[j].st_name != 0) {
					t_symbol* new_symbol = malloc(sizeof(t_symbol));
					new_symbol->index = j;
					new_symbol->sym = &symtab[j];
					new_symbol->sym32 = NULL;
					new_symbol->name = strtab + symtab[j].st_name;
					if (symtab[j].st_shndx > ehdr->e_shnum)
						new_symbol->shndx = ft_strdup("-1");
					else
						new_symbol->shndx = ft_itoa(symtab[j].st_shndx);
					new_symbol->type =
						_get_symbol_char(symtab[j], shdr, ehdr->e_shnum);
					if (!ft_strcmp(strtab + symtab[j].st_name, "_start"))
						new_symbol->type = 'T';
					new_symbol->value = ft_itoa(symtab[j].st_value);
					new_symbol->next = symbols;
					symbols = new_symbol;
				}
			}
			symbols = _sort64(symbols, flags);
			t_symbol* symbol = symbols;
			while (symbol) {
				if ((flags & FLAG_G) && ft_tolower(symbol->type) == symbol->type && symbol->type != 'w')
				{
					symbol = symbol->next;
					continue;
				}
				if ((is_debug(*symbol->sym) || ft_strchr("a", symbol->type)) &&
					!(flags & FLAG_A)) {
					symbol = symbol->next;
					continue;
				}
				if (!ft_strchr("Uvw", symbol->type) && !(flags & FLAG_U)){
					printf("%016lx %c %s\n", symbol->sym->st_value,
						   symbol->type, symbol->name);
					(void)print_Elf64_Shdr;
				} else if (flags & FLAG_U){
					if (ft_strchr("Uwv", symbol->type))
						printf("%16c %c %s\n", ' ', symbol->type, symbol->name);
				}
				 else
					printf("%16c %c %s\n", ' ', symbol->type, symbol->name);
				symbol = symbol->next;
			}
			clear_symbol_list(symbols);
		}
	}
}

int _nm32(void* ptr, int flags, struct stat* statbuff, char* filename) {
	bool symtab_found = false;
	Elf32_Ehdr* ehdr = (Elf32_Ehdr*)ptr;
	Elf32_Shdr* shdr;
	Elf32_Shdr* shstrtab;
	char* shstrtab_p;

	if (ehdr->e_shoff + sizeof(Elf32_Shdr) * ehdr->e_shnum >
		(size_t)statbuff->st_size)
		_file_format_no_recognized(filename, -1, ptr, statbuff);

	shdr = (Elf32_Shdr*)(ptr + ehdr->e_shoff);
	shstrtab = &shdr[ehdr->e_shstrndx];
	if (shstrtab->sh_offset + shstrtab->sh_size > (size_t)statbuff->st_size)
		_file_format_no_recognized(filename, -1, ptr, statbuff);

	shstrtab_p = (char*)(ptr + shstrtab->sh_offset);
	if (ehdr->e_shnum == 0 || ehdr->e_shstrndx == SHN_UNDEF)
		_file_format_no_recognized(filename, -1, ptr, statbuff);

	for (unsigned int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_name >= shstrtab->sh_size)
			_file_format_no_recognized(filename, -1, ptr, statbuff);
		if (!ft_strcmp(".symtab", shstrtab_p + shdr[i].sh_name)) {
			symtab_found = true;
			if (shdr[i].sh_offset + shdr[i].sh_size >
					(size_t)statbuff->st_size ||
				shdr[shdr[i].sh_link].sh_offset > (size_t)statbuff->st_size) {
				fprintf(stderr, "%s: %s: file format not recognized\n",
						PROGRAM_NAME, filename);
				close_file(-1, ptr, statbuff, statbuff->st_size);
			}
			Elf32_Sym* symtab = (Elf32_Sym*)(ptr + shdr[i].sh_offset);
			char* strtab = (char*)(ptr + shdr[shdr[i].sh_link].sh_offset);

			for (unsigned int j = 0; j < shdr[i].sh_size / sizeof(Elf32_Sym);
				 j++) {
				if ((size_t)(strtab + symtab[j].st_name) >=
					(size_t)(ptr + statbuff->st_size)) {
					fprintf(stderr, "%s: %s: file format not recognized\n",
							PROGRAM_NAME, filename);
					close_file(-1, ptr, statbuff, statbuff->st_size);
				}
			}
		}
	}
	if (!symtab_found) {
		fprintf(stderr, "%s: %s: no symbols\n", PROGRAM_NAME, filename);
		close_file(-1, ptr, statbuff, statbuff->st_size);
		return (EXIT_FAILURE);
	}
	print_symbols(ehdr, shstrtab_p, shdr, ptr, flags);
	return (EXIT_SUCCESS);
}

void print_symbols(Elf32_Ehdr* ehdr,
				   char* shstrtab_p,
				   Elf32_Shdr* shdr,
				   void* ptr,
				   int flags) {
	for (unsigned int i = 0; i < ehdr->e_shnum; i++) {
		if (ft_strcmp(".symtab", shstrtab_p + shdr[i].sh_name) == 0) {
			Elf32_Sym* symtab = (Elf32_Sym*)(ptr + shdr[i].sh_offset);
			char* strtab = (char*)(ptr + shdr[shdr[i].sh_link].sh_offset);
			t_symbol* symbols = NULL;
			for (unsigned int j = 0; j < shdr[i].sh_size / sizeof(Elf32_Sym);
				 j++) {
				if (symtab[j].st_name != 0) {
					t_symbol* new_symbol = malloc(sizeof(t_symbol));
					new_symbol->sym = NULL;
					new_symbol->index = j;
					new_symbol->sym32 = &symtab[j];
					new_symbol->name = strtab + symtab[j].st_name;
					if (symtab[j].st_shndx > ehdr->e_shnum)
						new_symbol->shndx = ft_strdup("-1");
					else
						new_symbol->shndx = ft_itoa(symtab[j].st_shndx);
					new_symbol->type =
						_get_symbol_char32(symtab[j], shdr, ehdr->e_shnum);

					new_symbol->value = ft_itoa(symtab[j].st_value);

					new_symbol->next = symbols;
					symbols = new_symbol;
				}
			}
			symbols = _sort32(symbols, flags);
			t_symbol* symbol = symbols;
			while (symbol) {
				if (is_debug32(*symbol->sym32) ||
					ft_strchr("a", symbol->type)) {
					symbol = symbol->next;
					continue;
				}
				if (flags & FLAG_G && ft_tolower(symbol->type) == symbol->type && symbol->type != 'w')
				{
					symbol = symbol->next;
					continue;
				}
				if (!ft_strchr("Uvw", symbol->type) && !(flags & FLAG_U)){

					if (symbol->type == 'C'){
						printf("%08x %c %s\n", symbol->sym32->st_size,
							   symbol->type, symbol->name);
					} else {
						printf("%08x %c %s\n", symbol->sym32->st_value,
							   symbol->type, symbol->name);
					}
				}
				else if (flags & FLAG_U){
					if (ft_strchr("Uwv", symbol->type))
						printf("%8c %c %s\n", ' ', symbol->type, symbol->name);
				} else
					printf("%8c %c %s\n", ' ', symbol->type, symbol->name);

				symbol = symbol->next;
			}
			clear_symbol_list(symbols);
		}
	}
}

void nm(char* filename, int flags) {
	int fd;
	struct stat statbuf;
	void* ptr;

	if ((fd = open(filename, O_RDONLY)) < 0) {
		fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, filename,
				strerror(errno));
		return;
	}
	if (fstat(fd, &statbuf) < 0) {
		fprintf(stderr, "%s: %s: %s\n", PROGRAM_NAME, filename,
				strerror(errno));
		close_file(fd, NULL, &statbuf, sizeof(Elf64_Ehdr));
		return;
	}
	if (S_ISDIR(statbuf.st_mode)) {
		fprintf(stderr, "%s: Warning: \'%s\' is a directory\n", PROGRAM_NAME,
				filename);
		close(fd);
		return;
	}
	if (!S_ISREG(statbuf.st_mode)) {
		fprintf(stderr, "%s: Warning: '%s' is not an ordinary file\n",
				PROGRAM_NAME, filename);
		close_file(fd, NULL, &statbuf, sizeof(Elf64_Ehdr));
		return;
	}
	if ((unsigned int)statbuf.st_size < (sizeof(Elf64_Ehdr))) {
		_file_format_no_recognized(filename, -1, NULL, &statbuf);
		return;
	}
	if ((ptr = mmap(0, sizeof(Elf64_Ehdr), PROT_READ, MAP_PRIVATE, fd, 0)) ==
		MAP_FAILED) {
		perror("mmap");
		close_file(fd, NULL, &statbuf, sizeof(Elf64_Ehdr));
		return;
	}
	munmap(ptr, sizeof(Elf64_Ehdr));
	if ((ptr = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) ==
		MAP_FAILED) {
		perror("mmap");
		close_file(fd, NULL, &statbuf, sizeof(Elf64_Ehdr));
		return;
	}
	if (ft_memcmp(ELFMAG, ptr, SELFMAG) != 0) {
		fprintf(stderr, "%s: %s: file format not recognized\n", PROGRAM_NAME,
				filename);
		close_file(fd, ptr, &statbuf, statbuf.st_size);
		return;
	}
	Elf64_Ehdr* ehdr = (Elf64_Ehdr*)ptr;
	if ((ehdr->e_ident[EI_CLASS] != ELFCLASS64 &&
		 ehdr->e_ident[EI_CLASS] != ELFCLASS32) ||
		ehdr->e_ident[EI_VERSION] != EV_CURRENT) {
		fprintf(stderr, "%s: %s: file format not recognized\n", PROGRAM_NAME,
				filename);
		close_file(fd, ptr, &statbuf, statbuf.st_size);
		return;
	}
	if (ehdr->e_ident[EI_CLASS] == ELFCLASS64) {
		_nm64(ptr, flags, &statbuf, filename);
	} else if (ehdr->e_ident[EI_CLASS] == ELFCLASS32) {
		_nm32(ptr, flags, &statbuf, filename);

	} else {
		fprintf(stderr, "%s: %s: file format not recognized\n", PROGRAM_NAME,
				filename);
	}
	close_file(fd, ptr, &statbuf, statbuf.st_size);
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
			case 'g':
				flags |= FLAG_G;
				break;
			default:
				ft_putstr_fd("Usage: ", 2);
				ft_putstr_fd(PROGRAM_NAME, 2);
				ft_putstr_fd(" [-flags] [file1, ..., file-n]\n", 2);
				exit(EXIT_FAILURE);
		}
	}
	file_count = argc - optind;
	if (file_count == 0) {
		nm("a.out", flags);
	} else {
		for (unsigned int i = 0; optind < argc; optind++, i++) {
			if (file_count > 1) {
				printf("%s:\n", argv[optind]);
			}
			nm(argv[optind], flags);
			if (optind < argc - 1)
				printf("\n");
		}
	}
}
