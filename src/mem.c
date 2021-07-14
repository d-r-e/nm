#include "../include/nm.h"

int is_elf(const char *memfile, struct stat *s)
{
	if (s->st_size < 4)
		return (FALSE);
	if (*memfile != 127)
	 	return (FALSE);
	if (ft_strncmp(memfile + 1, "ELF", 3))
		return (FALSE);
	return (TRUE);
}

void read_elf_header(const void* ptr) {
  // Either Elf64_Ehdr or Elf32_Ehdr depending on architecture.
  ElfW(Ehdr) header;

  if(ptr) {
    // read the header
    ft_memcpy(&header, ptr, sizeof(header));

    // check so its really an elf file
    if (ft_memcmp(header.e_ident, ELFMAG, (long unsigned int)SELFMAG) == 0) {
       printf("YASS\n");
    }
  }
}

int analyse_elf(const char *s)
{
	
	ft_puts(s);
	return (0);
}