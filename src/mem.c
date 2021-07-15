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

void read_elf_header(const void* ptr)
{
	ElfW(Ehdr) header;

	if(ptr)
	{
		ft_memcpy(&header, ptr, sizeof(header));
		if (ft_memcmp(header.e_ident, ELFMAG, (long unsigned int)SELFMAG) == 0)
		{
			 printf("YASS\n");
		}
	}
}

int analyse_elf(const char *s, const char *path)
{
	(void)path;
	printf("%s: %s: no symbols\n", BINARY, path);
	(void)s;
	return (0);
}