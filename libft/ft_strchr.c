/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 13:54:21 by darodrig          #+#    #+#             */
/*   Updated: 2019/11/05 14:05:07 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/***
 * ft_strchr - locate the first occurrence of a character in a string
 * @s: the string to be scanned
 * @c: the character to be searched for
 *
 * This function locates the first occurrence of the character c (converted to a
 * char) in the string pointed to by s. The terminating null character is
 * considered to be part of the string, so it can also be located to obtain a
 * pointer to the end of a string.
 *
 * Return: a pointer to the first occurrence of the character c in the string s,
 * or NULL if the character is not found.
 */
char	*ft_strchr(const char *s, int c)
{
	char *ptr;

	ptr = (char*)s;
	while (*ptr)
	{
		if (*ptr == c)
			return (ptr);
		ptr++;
	}
	if (*ptr == '\0' && c == '\0')
		return (ptr);
	return (NULL);
}
