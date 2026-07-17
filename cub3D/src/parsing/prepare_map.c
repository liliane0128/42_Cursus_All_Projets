/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:01 by hguo              #+#    #+#             */
/*   Updated: 2026/01/26 19:14:42 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static bool	find_player_position(t_club *club)
{
	int	y;
	int	x;

	y = 0;
	while (club->map.grid[y])
	{
		x = 0;
		while (club->map.grid[y][x])
		{
			if (club->map.grid[y][x] == 'N' || club->map.grid[y][x] == 'S'
				|| club->map.grid[y][x] == 'W' || club->map.grid[y][x] == 'E')
			{
				if (!is_surrounded_by_zero(club->map.grid, x, y))
					return (err_msg("Error: Player incorrectly placed."), \
						false);
				club->player.pos = club->map.grid[y][x];
				club->player.x = x + 0.5;
				club->player.y = y + 0.5;
				return (true);
			}
			x++;
		}
		y++;
	}
	return (false);
}

static bool	init_player_direction(t_club *club)
{
	if (club->player.pos == 'N')
	{
		club->player.dir_x = 0;
		club->player.dir_y = -1;
	}
	else if (club->player.pos == 'S')
	{
		club->player.dir_x = 0;
		club->player.dir_y = 1;
	}
	else if (club->player.pos == 'W')
	{
		club->player.dir_x = -1;
		club->player.dir_y = 0;
	}
	else if (club->player.pos == 'E')
	{
		club->player.dir_x = 1;
		club->player.dir_y = 0;
	}
	else
		return (false);
	club->player.plane_x = -club->player.dir_y * FOV;
	club->player.plane_y = club->player.dir_x * FOV;
	return (true);
}

static bool	is_cell_closed(t_club *club, int x, int y)
{
	char	**grid;

	grid = club->map.grid;
	if (y == 0 || y == club->map.height - 1 || \
		x == 0 || x == (int)ft_strlen(grid[y]) - 1)
		return (false);
	if (ft_is_whitespace(grid[y - 1][x])
		|| ft_is_whitespace(grid[y + 1][x])
		|| ft_is_whitespace(grid[y][x - 1])
		|| ft_is_whitespace(grid[y][x + 1]))
		return (false);
	return (true);
}

static bool	check_map_closed(t_club *club)
{
	int		y;
	int		x;
	char	c;

	y = 0;
	while (club->map.grid[y])
	{
		x = 0;
		while (club->map.grid[y][x])
		{
			c = club->map.grid[y][x];
			if (c == '0' || c == 'N' || c == 'S'
				|| c == 'E' || c == 'W')
			{
				if (!is_cell_closed(club, x, y))
					return (false);
			}
			x++;
		}
		y++;
	}
	return (true);
}

bool	prepare_map(t_club *club)
{
	char	**map_copy;

	if (!club || !club->map.grid)
		return (false);
	if (!pad_map(club))
		return (false);
	map_copy = copy_map(club);
	if (!map_copy)
		return (false);
	if (!find_player_position(club) || !init_player_direction(club))
		return (free_array(map_copy), false);
	if (!check_map_closed(club))
	{
		err_msg("ERROR: map is not surrounded by walls");
		return (free_array(map_copy), false);
	}
	free_array(map_copy);
	return (true);
}
