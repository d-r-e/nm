#include "libft.h"

void ft_puts(const char *s)
{
	write(1, s, ft_strlen(s));
	write(1, "\n", 1);
}