#include "../include/nm.h"

size_t ft_strlen(const char *s)
{
	size_t i;

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

void ft_putnbr(int n)
{
	printf("%d", n);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	i = 0;
	if (n == 0)
		return (0);
	while ((unsigned char)s1[i] == (unsigned char)s2[i] && s1[i] && s2[i] \
			&& i < n)
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int strerr(const char *s)
{
	write(2, s, ft_strlen(s));
	return (-1);
}