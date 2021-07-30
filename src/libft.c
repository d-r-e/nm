#include "../include/nm.h"

void	ft_bzero(void *s, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		((char *)s)[i] = 0;
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	for ( ; i < n; i++)
		dest[i] = '\0';
	return dest;
}

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	if (s1 == s2 || n == 0)
		return (0);
	while (n--)
	{
		if (*(char*)s1 != *(char*)s2){
			return (*(char*)s1 - *(char*)s2);
		}
		s1++;
		s2++;
	}
	return (0);
}

void	*ft_memcpy(void *str1, const void *str2, size_t n)
{
	unsigned int		i;
	unsigned char		*dest;
	unsigned const char	*str;

	if (str1 == NULL && str2 == NULL)
		return (NULL);
	i = 0;
	dest = (unsigned char *)str1;
	str = (unsigned char *)str2;
	while (i < n)
	{
		*((char *)dest + i) = *((char *)str + i);
		i++;
	}
	return (dest);
}

/*
 * little endian to big endian
*/
unsigned int ft_ltob(unsigned int n)
{
	char lit[4];
	char big[4];
	int	ret;

	ft_memcpy((char*)lit, (const char*)&n, 4);
	big[0] = lit[3];
	big[1] = lit[2];
	big[2] = lit[1];
	big[3] = lit[0];
	ft_memcpy((char*)&ret, (const char*)big, 4);
	return (ret);
}

unsigned long int ft_lltob(unsigned long int n)
{
	char lit[8];
	char big[8];
	long int	ret;

	ft_memcpy((char*)lit, (const char*)&n, 8);
	for (int i = 0; i < 8; ++i)
	{
		big[i] = lit[7 - i];
	}
	ft_memcpy((char*)&ret, (const char*)big, 8);
	return (ret);
}
