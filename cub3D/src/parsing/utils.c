/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:09 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:30:10 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

bool	is_empty_line(char *line)
{
	int	i;

	if (!line)
		return (true);
	i = 0;
	while (line[i])
	{
		if (!ft_is_whitespace(line[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	is_cub_file(char *arg)
{
	int	len;
	int	fd;

	if (!arg)
		return (false);
	len = ft_strlen(arg);
	if (len < 4 || ft_strcmp(arg + len - 4, ".cub") != 0)
		return (false);
	fd = open(arg, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

char	**copy_map(t_club *club)
{
	char	**map;
	int		i;

	map = malloc(sizeof(char *) * (club->map.height + 1));
	if (!map)
		return (NULL);
	i = 0;
	while (i < club->map.height)
	{
		map[i] = ft_strdup(club->map.grid[i]);
		if (!map[i])
			return (free_array(map), NULL);
		i++;
	}
	map[i] = NULL;
	return (map);
}
