/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darodrig <darodrig@wakanda>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 09:58:19 by darodrig          #+#    #+#             */
/*   Updated: 2019/11/27 14:13:41 by darodrig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_line(int fd, char **s, char **line, int rd)
{
	char	*tmp;
	char	*nl;

	if (!s[fd] && !rd)
	{
		*line = ft_strdup("");
		return (0);
	}
	if ((nl = ft_strchr(s[fd], '\n')))
	{
		*nl = '\0';
		*line = s[fd];
		tmp = ft_strdup(++nl);
		free(s[fd]);
		s[fd] = tmp;
		return (1);
	}
	else
	{
		*line = s[fd];
		s[fd] = NULL;
		return (0);
	}
}

int			get_next_line(int fd, char **line)
{
	char		buff[BUFFER_SIZE + 1];
	static char	*s[4096];
	int			rd;
	char		*temp;

	if (!line || fd < 0 || fd > 4095 || (rd = read(fd, buff, 0)) == -1
			|| BUFFER_SIZE < 1)
		return (-1);
	while ((rd = read(fd, buff, BUFFER_SIZE)) > 0)
	{
		buff[rd] = '\0';
		if (s[fd] == NULL)
			s[fd] = ft_strdup("");
		temp = ft_strjoin(s[fd], buff);
		free(s[fd]);
		s[fd] = temp;
		if (ft_strchr(s[fd], '\n'))
			break ;
	}
	if (rd < 0)
		return (-1);
	rd = get_line(fd, s, line, rd);
	return (rd);
}
