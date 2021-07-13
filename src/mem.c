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

int analyse(const char *s)
{
	if (!s)
	{
		printf("Error: mmap.");
		return (-1);
	}		
	ft_putstr(s);
	return (0);
}