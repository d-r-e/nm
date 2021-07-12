#include "../include/nm.h"

size_t ft_strlen(const char *s)
{
	int i;

	i = 0;
	while(s && s[i])
		i++;
	return i;
}

void ft_putstr(const char *s)
{
	write(1, s, ft_strlen(s));
}

void ft_puts(const char *s)
{
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
}