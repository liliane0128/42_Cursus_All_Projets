/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:29:53 by hguo              #+#    #+#             */
/*   Updated: 2026/01/26 17:00:24 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

char	*pad_line(char *line, int new_len)
{
	int		i;
	char	*new_line;

	new_line = malloc(sizeof(char) * (new_len + 1));
	if (!new_line)
		return (NULL);
	i = 0;
	while (line[i] && line[i] != '\n' && line[i] != '\r')
	{
		new_line[i] = line[i];
		i++;
	}
	while (i < new_len)
		new_line[i++] = ' ';
	new_line[i] = '\0';
	free(line);
	return (new_line);
}

static bool	check_sides(t_club *club)
{
	int		i;
	int		start;
	int		end;
	char	*line;

	i = 0;
	while (club->map.grid[i])
	{
		line = club->map.grid[i];
		if (!line || ft_strlen(line) == 0)
			return (false);
		end = ft_strlen(line) - 1;
		start = 0;
		while (line[start] && ft_is_whitespace(line[start]))
			start++;
		if (!line[start] || line[start] != '1')
			return (false);
		while (end >= 0 && ft_is_whitespace(line[end]))
			end--;
		if (end < 0 || line[end] != '1')
			return (false);
		i++;
	}
	return (true);
}

static bool	check_map_size(t_club *club)
{
	int	y;

	if (club->map.height < 3 || club->map.width < 3)
		return (false);
	y = 0;
	while (club->map.grid[y])
	{
		if (!ft_strchr(club->map.grid[y], '0') \
			&& !ft_strchr(club->map.grid[y], '1') \
			&& !ft_strchr(club->map.grid[y], 'N') \
			&& !ft_strchr(club->map.grid[y], 'S') \
			&& !ft_strchr(club->map.grid[y], 'W') \
			&& !ft_strchr(club->map.grid[y], 'E') \
			&& !ft_strchr(club->map.grid[y], '2') \
			&& !ft_strchr(club->map.grid[y], '3') \
			&& !ft_strchr(club->map.grid[y], '4') \
			&& !ft_strchr(club->map.grid[y], '5') \
			&& !ft_strchr(club->map.grid[y], 'D'))
			return (false);
		y++;
	}
	return (true);
}

bool	is_map_at_the_end(t_club *club, char **file)
{
	int	i;

	if (!file || !club->map.grid)
		return (false);
	i = club->map.map_end_index + 1;
	while (file[i])
	{
		if (!is_empty_line(file[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	check_map_valid(t_club *club)
{
	if (!club->map.grid)
		return (false);
	if (!check_valid_chars(club))
		return (err_msg("Error: map has invalid character"), false);
	if (!check_first_last_row(club) || !check_sides(club))
		return (err_msg("Error: map is not surrounded by walls"), false);
	if (!check_map_size(club))
		return (err_msg("Error: Map dimensions are invalid"), false);
	if (!check_player_count(club))
		return (err_msg("Error: map should have one and only one player"), \
			false);
	return (true);
}
