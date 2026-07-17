/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 19:00:54 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/26 19:01:44 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	get_expanded_len(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (line[i])
	{
		if (line[i] == '\t')
			len += 4;
		else
			len++;
		i++;
	}
	return (len);
}

char	*strdup_expanded(char *line)
{
	char	*new;
	int		i;
	int		j;

	new = malloc(sizeof(char) * (get_expanded_len(line) + 1));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '\t')
		{
			new[j++] = ' ';
			new[j++] = ' ';
			new[j++] = ' ';
			new[j++] = ' ';
		}
		else
			new[j++] = line[i];
		i++;
	}
	new[j] = '\0';
	return (new);
}

bool	is_surrounded_by_zero(char **map, int x, int y)
{
	if (map[y - 1][x] == '0' || map[y + 1][x] == '0' \
		|| map[y][x - 1] == '0' || map[y][x + 1] == '0')
		return (true);
	return (false);
}
