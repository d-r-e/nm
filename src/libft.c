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
	const unsigned char		*str1;
	const unsigned char		*str2;

	if (s1 == s2 || n == 0)
		return (0);
	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	while (n--)
	{
		if (*str1 != *str2)
			return (*str1 - *str2);
			str1++;
			str2++;
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
	char buf[8];

// set the byte array from smallest to largest byte
for(int i = 0; i < 8; ++i) {
    buf[i] = (n >> i*8) & 0xFF;
}

// build the new long from largest to smallest byte (reversed)
	unsigned long l = ((buf[0] & 0xFFL) << 56) | \
         ((buf[2] & 0xFFL) << 40) | \
         ((buf[1] & 0xFFL) << 48) | \
         ((buf[3] & 0xFFL) << 32) | \
         ((buf[4] & 0xFFL) << 24) | \
         ((buf[5] & 0xFFL) << 16) | \
         ((buf[6] & 0xFFL) <<  8) | \
         ((buf[7] & 0xFFL) <<  0) ;
	return (l);
}
