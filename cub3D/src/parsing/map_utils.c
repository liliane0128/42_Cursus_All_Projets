/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:29:43 by hguo              #+#    #+#             */
/*   Updated: 2026/01/26 12:51:02 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

bool	is_valid_char(char c, bool bonus)
{
	if (c == '0' || c == '1' || c == ' ' || c == 'N'\
		|| c == 'W' || c == 'E' || c == 'S' || ft_is_whitespace(c))
		return (true);
	if (bonus && ((c >= '2' && c <= '5') || c == 'D'))
		return (true);
	return (false);
}

bool	check_valid_chars(t_club *club)
{
	int	y;
	int	x;

	y = 0;
	while (y < club->map.height)
	{
		x = 0;
		while (club->map.grid[y][x] != '\0')
		{
			if (!is_valid_char(club->map.grid[y][x], true))
				return (false);
			x++;
		}
		y++;
	}
	return (true);
}

bool	check_player_count(t_club *club)
{
	int	count;
	int	y;
	int	x;

	count = 0;
	y = 0;
	while (club->map.grid[y])
	{
		x = 0;
		while (club->map.grid[y][x])
		{
			if (club->map.grid[y][x] == 'N' || club->map.grid[y][x] == 'S' \
				|| club->map.grid[y][x] == 'E' || club->map.grid[y][x] == 'W')
				count++;
			x++;
		}
		y++;
	}
	if (count != 1)
		return (false);
	return (true);
}

bool	check_first_last_row(t_club *club)
{
	int	x;
	int	last_y;

	if (!club->map.height)
		return (false);
	last_y = club->map.height - 1;
	x = 0;
	while (club->map.grid[0][x])
	{
		if (club->map.grid[0][x] != '1' \
			&& !ft_is_whitespace(club->map.grid[0][x]))
			return (false);
		x++;
	}
	x = 0;
	while (club->map.grid[last_y][x])
	{
		if (club->map.grid[last_y][x] != '1' \
			&& !ft_is_whitespace(club->map.grid[last_y][x]))
			return (false);
		x++;
	}
	return (true);
}

bool	pad_map(t_club *club)
{
	int		i;
	char	*new_line;

	i = 0;
	while (club->map.grid[i])
	{
		new_line = pad_line(club->map.grid[i], club->map.width);
		if (!new_line)
			return (err_msg("Error: failed to pad map line"), false);
		club->map.grid[i] = new_line;
		i++;
	}
	return (true);
}
