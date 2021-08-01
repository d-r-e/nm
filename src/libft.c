#include "../include/nm.h"

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
