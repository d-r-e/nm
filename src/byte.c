#include "../include/nm.h"

static char	*ft_strrev(char *str, size_t len)
{
	size_t	i;
	char	tmp;

	i = 0;
	while (i < len)
	{
		tmp = str[i];
		str[i] = str[len-1];
		str[len-1] = tmp;
		i++;
		len--;
	}
	return (str);
}

unsigned long
ft_swaplong(unsigned long inv)
{
    char swapped[sizeof(unsigned long)];
    unsigned long ret;

    ft_memcpy(swapped, &inv, sizeof(inv));
    ft_strrev(swapped, sizeof(unsigned long));
    ft_memcpy(&ret, swapped, sizeof(ret));
    return(ret);
}

unsigned long long
ft_swaplonglong(unsigned long long inv)
{
    char swapped[sizeof(unsigned long)];
    unsigned long long ret;

    ft_memcpy(swapped, &inv, sizeof(inv));
    ft_strrev(swapped, sizeof(unsigned long long));
    ft_memcpy(&ret, swapped, sizeof(ret));
    return (ret);
}

unsigned int
ft_swapint(unsigned int inv)
{
    return(ft_ltob(inv));
}