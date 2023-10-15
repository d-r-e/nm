/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/05 14:47:53 by darodrig          #+#    #+#             */
/*   Updated: 2022/04/11 14:12:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief The ft_strrchr() function returns a pointer to the last occurrence of
 *  the character c in the string s.
 * @param s string
 * @param c character to find
 * @return char* or NULL when no ocurrence is found
 */
char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;
	int		i;

	i = ft_strlen(s);
	ptr = (char *)s + i;
	if (c == '\0')
		return (ptr);
	while (i >= 0)
	{
		if (*ptr == c)
			return (ptr);
		ptr--;
		i--;
	}
	return (NULL);
}
