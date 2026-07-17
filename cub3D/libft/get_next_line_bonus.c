/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 14:43:24 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/19 18:31:08 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	clear(char *buff, int i)
{
	if (i < 0)
		i = 0;
	while (i <= BUFFER_SIZE)
		buff[i++] = 0;
	return (1);
}

static int	nl_index(const char *str, int bytes)
{
	int	i;

	i = -1;
	if (!str)
		return (-1);
	while (str[++i])
	{
		if (str[i] == '\n')
			return (i + 1);
	}
	if (bytes < BUFFER_SIZE)
		return (bytes);
	return (-1);
}

char	*str_join(char *s1, const char *s2, int len)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	while (s1 && s1[i])
		i++;
	j = 0;
	while (j < len && s2[j])
		j++;
	str = malloc(i + j + 1);
	if (!str)
		return (free(s1), NULL);
	len = -1;
	while (++len < i)
		str[len] = s1[len];
	len = -1;
	while (++len < j)
		str[i + len] = s2[len];
	str[i + len] = 0;
	return (free(s1), str);
}

char	*get_next_line(int fd)
{
	static char	buff[1024][BUFFER_SIZE + 1];
	char		*line;
	int			b;
	int			i;
	int			j;

	if (fd < 0 || fd > 1023)
		return (NULL);
	free((j = -1, line = NULL, b = BUFFER_SIZE, NULL));
	while (nl_index(buff[fd], b) == -1)
	{
		line = str_join(line, buff[fd], BUFFER_SIZE);
		b = read(fd, buff[fd], BUFFER_SIZE);
		if (clear(buff[fd], b) && (b < 0 || !line || (!b && !*line)))
			return (clear(buff[fd], 0), free(line), NULL);
	}
	i = nl_index(buff[fd], b);
	line = str_join(line, buff[fd], i);
	while (buff[fd][++j + i])
		buff[fd][j] = buff[fd][j + i];
	buff[fd][j] = 0;
	if (!line || !*line)
		return (free(line), NULL);
	return (line);
}
