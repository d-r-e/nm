/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 14:55:30 by darodrig          #+#    #+#             */
/*   Updated: 2019/11/14 12:26:33 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/nm.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	if (!dst && !src && n > 0)
		return (NULL);
	if ((void*)(src) >= (void*)(g_mach.mem) && (void*)(src) < (void*)(g_mach.mem + g_mach.s.st_size) && \
	(void*)(src + n) > (void*)g_mach.mem + g_mach.s.st_size)
		exit (-1);

	i = 0;
	while (i < n)
	{
		((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
		i++;
	}
	return ((unsigned char*)dst);
}
