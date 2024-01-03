#include <nm.h>

char _get_symbol_char(Elf64_Sym sym, Elf64_Shdr* shdr, size_t shnum) {
	char c = 0;
	unsigned char type = ELF64_ST_TYPE(sym.st_info);
	unsigned char bind = ELF64_ST_BIND(sym.st_info);

	if (sym.st_shndx != SHN_UNDEF && sym.st_shndx != SHN_ABS && sym.st_shndx > shnum)
		return ('?');
	switch (type) {
		case STT_TLS:
			if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY ||
				shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY ||
				shdr[sym.st_shndx].sh_type == SHT_PREINIT_ARRAY ||
				shdr[sym.st_shndx].sh_type == SHT_PROGBITS)
				c = 'D';
			else if (bind == STB_LOCAL &&
					 shdr[sym.st_shndx].sh_type == SHT_NOBITS)
				c = 'B';
			else if (bind == STB_WEAK)
				c = 'W';
			else
				c = 'U';
			break;
		case STT_NOTYPE:
			if (sym.st_shndx == SHN_UNDEF && bind == STB_GLOBAL)
				c = 'U';
			else if (bind == STB_LOCAL &&
					 shdr[sym.st_shndx].sh_type == SHT_NOBITS)
				c = 'B';
			else if (!sym.st_value && bind != STB_WEAK) {
				if (bind == STB_GLOBAL &&
					(shdr[sym.st_shndx].sh_type == SHT_PROGBITS ||
					 shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY))
					c = 'D';
				else
					c = 'R';
			} else if (bind == STB_LOCAL) {
				if (shdr[sym.st_shndx].sh_flags & SHF_INFO_LINK)
					c = 'T';
				else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS) {
					if (shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR)
						c = 'T';
					else
						c = 'R';
				} else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY)
					c = 'D';
				else
					c = '?';
			} else if (bind == STB_WEAK) {
				if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS)
					c = 'W';
				else if (shdr[sym.st_shndx].sh_type == SHT_NULL)
					c = 'w';
			} else if (bind == STB_GLOBAL &&
					   (shdr[sym.st_shndx].sh_type == SHT_PROGBITS ||
						shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY))
				c = 'D';
			else if (bind == STB_GLOBAL &&
					 shdr[sym.st_shndx].sh_type == SHT_NOBITS)
				c = 'B';
			else
				c = '?';
			break;
		case STT_OBJECT:
			if (bind == STB_WEAK) {
				if (sym.st_shndx == SHN_UNDEF)
					c = 'v';
				else
					c = 'V';
			} else if (sym.st_shndx == SHN_UNDEF)
				c = 'U';
			else if (sym.st_shndx == SHN_COMMON)
				c = 'C';
			else if (bind == STB_LOCAL && sym.st_shndx &&
					 shdr[sym.st_shndx].sh_type == SHT_NOBITS)
				c = 'B';
			else if (bind == STB_LOCAL && sym.st_shndx &&
					 !shdr[sym.st_shndx].sh_flags)
				c = 'N';
			else if (bind == STB_LOCAL && sym.st_shndx &&
					 !(shdr[sym.st_shndx].sh_flags & SHF_WRITE &&
					   (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY ||
						shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY ||
						shdr[sym.st_shndx].sh_type == SHT_DYNAMIC ||
						shdr[sym.st_shndx].sh_type == SHT_PROGBITS)))
				c = 'R';
			else if (bind == STB_LOCAL && sym.st_shndx &&
					 !(shdr[sym.st_shndx].sh_flags & SHF_WRITE))
				c = 'N';
			else if (bind == STB_LOCAL &&
					 (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY ||
					  shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY ||
					  shdr[sym.st_shndx].sh_type == SHT_DYNAMIC ||
					  shdr[sym.st_shndx].sh_type == SHT_PROGBITS))
				c = 'D';
			else if (bind == STB_GLOBAL &&
					 ELF64_ST_VISIBILITY(sym.st_other) == STV_HIDDEN &&
					 !(shdr[sym.st_shndx].sh_flags & SHF_WRITE))
				c = 'R';
			else if (!bind && shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR)
				c = 'D';
			else if (!sym.st_value && bind != STB_WEAK) {
				if (sym.st_shndx <= shnum && !shdr[sym.st_shndx].sh_addr &&
					!(shdr[sym.st_shndx].sh_flags & SHF_WRITE))
					c = 'R';
				else if (sym.st_shndx <= shnum && !shdr[sym.st_shndx].sh_addr &&
						 (shdr[sym.st_shndx].sh_flags &
						  (SHF_WRITE | SHF_ALLOC)))
					c = 'D';
				else
					c = 'A';
			} else {
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
			else if (bind != STB_GLOBAL &&
					 shdr[sym.st_shndx].sh_type == SHT_NULL &&
					 !sym.st_shndx == SHN_UNDEF)
				c = 'w';
			else if (sym.st_shndx == SHN_UNDEF)
				return 'U';
			else if (bind == STB_WEAK)
				return 'W';
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
	if (sym.st_shndx == SHN_ABS && c != 'r') {
		c = 'A';
	}
	if (bind == STB_WEAK && c != 'A' && c != 'V' &&
		shdr[sym.st_shndx].sh_addr && shdr[sym.st_shndx].sh_type == SHT_NULL) {
		c = ft_tolower(c);
	} else if (((bind == STB_LOCAL) ||
				(bind == STB_WEAK && c != 'W' && c != 'V')) &&
			   c != 'U') {
		c = ft_tolower(c);
	}
	return c;
}

char _get_symbol_char32(Elf32_Sym sym, Elf32_Shdr* shdr, size_t shnum) {
	char c = 0;
	unsigned char type = ELF32_ST_TYPE(sym.st_info);
	unsigned char bind = ELF32_ST_BIND(sym.st_info);

	switch (type) {
		case STT_TLS:
			if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY ||
				shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY ||
				shdr[sym.st_shndx].sh_type == SHT_PREINIT_ARRAY ||
				shdr[sym.st_shndx].sh_type == SHT_PROGBITS)
				c = 'D';
			else if (bind == STB_LOCAL &&
					 shdr[sym.st_shndx].sh_type == SHT_NOBITS)
				c = 'B';
			else if (bind == STB_WEAK)
				c = 'W';
			else
				c = 'U';
			break;
		case STT_NOTYPE:
			if (sym.st_shndx == SHN_UNDEF && bind == STB_GLOBAL)
				c = 'U';
			else if (bind == STB_LOCAL &&
					 shdr[sym.st_shndx].sh_type == SHT_NOBITS)
				c = 'B';
			else if (!sym.st_value && bind != STB_WEAK) {
				if (bind == STB_GLOBAL &&
					(shdr[sym.st_shndx].sh_type == SHT_PROGBITS ||
					 shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY))
					c = 'D';
				// else
				// 	return 'A';
			} else if (bind == STB_LOCAL) {
				if (shdr[sym.st_shndx].sh_flags & SHF_INFO_LINK)
					c = 'T';
				else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS) {
					if (shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR)
						c = 'T';
					else
						c = 'R';
				} else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY)
					c = 'D';
				else
					c = '?';
			} else if (bind == STB_WEAK) {
				if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS)
					c = 'W';
				else if (shdr[sym.st_shndx].sh_type == SHT_NULL)
					c = 'w';
			} else if (bind == STB_GLOBAL &&
					   (shdr[sym.st_shndx].sh_type == SHT_PROGBITS ||
						shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY))
				c = 'D';
			else if (bind == STB_GLOBAL &&
					 shdr[sym.st_shndx].sh_type == SHT_NOBITS)
				c = 'B';
			else
				c = '?';
			break;
		case STT_OBJECT:
			if (bind == STB_WEAK) {
				if (sym.st_shndx == SHN_UNDEF)
					c = 'v';
				else
					c = 'V';
			} else if (sym.st_shndx == SHN_UNDEF)
				c = 'U';
			else if (sym.st_shndx == SHN_COMMON)
				c = 'C';
			else if ((bind == STB_LOCAL || bind == STB_GLOBAL) && sym.st_shndx &&
					 shdr[sym.st_shndx].sh_type == SHT_NOBITS)
				c = 'B';
			else if (bind == STB_LOCAL && sym.st_shndx &&
					 !shdr[sym.st_shndx].sh_flags)
				c = 'N';
			else if (bind == STB_LOCAL && sym.st_shndx &&
					 !(shdr[sym.st_shndx].sh_flags & SHF_WRITE &&
					   (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY ||
						shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY ||
						shdr[sym.st_shndx].sh_type == SHT_DYNAMIC ||
						shdr[sym.st_shndx].sh_type == SHT_PROGBITS)))
				c = 'R';
			else if (bind == STB_LOCAL && sym.st_shndx &&
					 !(shdr[sym.st_shndx].sh_flags & SHF_WRITE))
				c = 'N';
			else if (bind == STB_LOCAL &&
					 (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY ||
					  shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY ||
					  shdr[sym.st_shndx].sh_type == SHT_DYNAMIC ||
					  shdr[sym.st_shndx].sh_type == SHT_PROGBITS))
				c = 'D';
			else if (bind == STB_GLOBAL &&
					 ELF64_ST_VISIBILITY(sym.st_other) == STV_HIDDEN &&
					 !(shdr[sym.st_shndx].sh_flags & SHF_WRITE))
				c = 'R';
			else if (!bind && shdr[sym.st_shndx].sh_flags & SHF_EXECINSTR)
				c = 'D';
			else if (!sym.st_value && bind != STB_WEAK) {
				if (sym.st_shndx <= shnum && !shdr[sym.st_shndx].sh_addr &&
					!(shdr[sym.st_shndx].sh_flags & SHF_WRITE))
					c = 'R';
				else if (sym.st_shndx <= shnum && !shdr[sym.st_shndx].sh_addr &&
						 (shdr[sym.st_shndx].sh_flags &
						  (SHF_WRITE | SHF_ALLOC)))
					c = 'D';
				else
					c = 'A';
			} else if (sym.st_shndx <= shnum) {
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
			else if (bind != STB_GLOBAL &&
					 shdr[sym.st_shndx].sh_type == SHT_NULL &&
					 !sym.st_shndx == SHN_UNDEF)
				c = 'w';
			else if (sym.st_shndx == SHN_UNDEF)
				return 'U';
			else if (bind == STB_WEAK)
				return 'W';
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
	if (sym.st_shndx == SHN_ABS) {
		c = 'A';
	}
	if (bind == STB_WEAK && c != 'A' && c != 'V' &&
		shdr[sym.st_shndx].sh_addr && shdr[sym.st_shndx].sh_type == SHT_NULL) {
		c = ft_tolower(c);
	} else if (((bind == STB_LOCAL) ||
				(bind == STB_WEAK && c != 'W' && c != 'V')) &&
			   c != 'U') {
		c = ft_tolower(c);
	}
	return c;
}

static char print_elf_shdr_flags(Elf64_Xword flags) {
	if (flags & SHF_WRITE)
		return ('W');
	else if (flags & SHF_ALLOC)
		return ('A');
	else if (flags & SHF_EXECINSTR)
		return ('X');
	else
		return (' ');
}

void print_Elf64_Shdr(Elf64_Shdr *shdr) {
	printf("Elf64_Word  sh_name: %d\n", shdr->sh_name);
	printf("Elf64_Word  sh_type: %d\n", shdr->sh_type);
	printf("Elf64_Xword sh_flags: %08lx %c\n", shdr->sh_flags,
		   print_elf_shdr_flags(shdr->sh_flags));
	printf("Elf64_Addr  sh_addr: %p\n", (void*)shdr->sh_addr);
	printf("Elf64_Off   sh_offset: %ld\n", shdr->sh_offset);
	printf("Elf64_Xword sh_size: %ld\n", shdr->sh_size);
	printf("Elf64_Word  sh_link: %d\n", shdr->sh_link);
	printf("Elf64_Word  sh_info: %d\n", shdr->sh_info);
	printf("Elf64_Xword sh_addralign: %ld\n", shdr->sh_addralign);
	printf("Elf64_Xword sh_entsize: %ld\n", shdr->sh_entsize);
}

