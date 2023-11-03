/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 18:22:42 by darodrig          #+#    #+#             */
/*   Updated: 2019/11/06 18:47:50 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * ft_strnstr - locate a substring in a string
 * @haystack: the string to be searched
 * @needle: the substring to be located
 * @len: the number of characters to be analyzed in the haystack
 *
 * This function locates the first occurrence of the null-terminated string
 * needle in the string haystack, where not more than len characters are
 * searched. Characters that appear after a '\0' character are not searched.
 *
 * Return: If needle is an empty string, haystack is returned. If needle occurs
 * nowhere in haystack, NULL is returned. Otherwise, a pointer to the first
 * character of the first occurrence of needle is returned.
 */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (*needle == '\0')
		return ((char*)haystack);
	i = 0;
	while (haystack[i] != '\0' && i < len)
	{
		j = 0;
		if (haystack[i] == needle[j] &&
				(i + ft_strlen(needle)) <= len)
		{
			while (needle[j] == haystack[i + j] && needle[j] != '\0')
				j++;
			if (j == ft_strlen(needle))
				return ((char*)&(haystack[i]));
		}
		i++;
	}
	return (NULL);
}
