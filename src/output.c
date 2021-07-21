#include "../include/nm.h"

int no_such_file(const char *s)
{
	ft_putstr(BINARY);
	ft_putstr(": \'");
	ft_putstr(s);
	ft_putstr("\': ");
	ft_puts(NO_SUCH_FILE);
	return (-1);
}

int permission_denied(const char *s)
{
	ft_putstr(BINARY);
	ft_putstr(": \'");
	ft_putstr(s);
	ft_putstr("\': ");
	ft_puts(PERMISSION_DENIED);
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