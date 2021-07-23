#include "../include/nm.h"

int file_error(const char *file, const char *error)
{
	printf("%s: \'%s\': %s\n", BINARY, file, error);
	return (-1);
}

int ft_strerr(const char *s)
{
	ft_putstr(BINARY);
	ft_putstr(": \'");
	ft_putstr(s);
	ft_putstr("\': ");
	ft_puts(STAT_ERROR);
	return (-1);
}