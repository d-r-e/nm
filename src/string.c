#include "../inc/nm.h"

size_t ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void ft_puts(const char *s)
{
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
}