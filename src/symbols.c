#include <nm.h>

char _get_symbol_char(Elf64_Sym sym, Elf64_Shdr* shdr) {
  char c;
  unsigned char type = ELF64_ST_TYPE(sym.st_info);
  unsigned char bind = ELF64_ST_BIND(sym.st_info);

  // return 'C';
  switch (type) {
    case STT_NOTYPE:
      if (!sym.st_value && bind != STB_WEAK){
        return 'A';
      }
      if (sym.st_shndx == SHN_UNDEF && bind == STB_GLOBAL)
        c = 'U';
      else if (bind == STB_LOCAL) {
        if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS) c = 'R';
        else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY) c = 'D';
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
      else if (bind == STB_GLOBAL && shdr[sym.st_shndx].sh_type == SHT_NOBITS)
        c = 'B';
      else
        c = '?';
      break;
    case STT_OBJECT:
      // printf("%08x\n",  sym.st_other);
      // printf("%d\n",ELF64_ST_VISIBILITY(sym.st_other));
      if (bind == STB_WEAK) c = 'V';
      else if (bind == STB_LOCAL &&
               (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY ||
                shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY ||
                shdr[sym.st_shndx].sh_type == SHT_DYNAMIC))
        c = 'D';
      else if (bind == STB_GLOBAL && ELF64_ST_VISIBILITY(sym.st_other) == STV_HIDDEN && !(shdr[sym.st_shndx].sh_flags & SHF_WRITE))
        c = 'R';          
      else if (!sym.st_value && bind!= STB_WEAK) {
        return 'A';
      }
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
  if (sym.st_shndx == SHN_ABS) {
    c = 'A';
  }
  if (type == STT_NOTYPE && bind == STB_WEAK &&
      shdr[sym.st_shndx].sh_type == SHT_NULL) {
    c = ft_tolower(c);
  } else if ((bind == STB_LOCAL || (bind == STB_WEAK && c != 'W')) &&
             c != 'U' && c != 'V') {
    c = ft_tolower(c);
  }
  return c;
}

void print_type_bind_shn(Elf64_Shdr* shdr,
                         unsigned char type,
                         unsigned char bind) {
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


void print_Elf64_Shdr(Elf64_Shdr* shdr) {
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