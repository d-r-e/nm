#include "../include/nm.h"

static char	*ft_strrev(char *str, size_t len)
{
	char	tmp;
	size_t		i;

    len--;
    i = 0;
	while (len >= i)
	{
		tmp = str[i];
		str[i] = str[len];
		str[len] = tmp;
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

long long
ft_swaplonglong(
long long ll)
{
	union {
	    char c[8];
	    long long ll;
	} in, out;
	in.ll = ll;
	out.c[0] = in.c[7];
	out.c[1] = in.c[6];
	out.c[2] = in.c[5];
	out.c[3] = in.c[4];
	out.c[4] = in.c[3];
	out.c[5] = in.c[2];
	out.c[6] = in.c[1];
	out.c[7] = in.c[0];
	return(out.ll);
}

unsigned int
ft_swapint(unsigned int inv)
{
    return(ft_ltob(inv));
}