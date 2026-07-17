/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_club.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:27:04 by hguo              #+#    #+#             */
/*   Updated: 2026/01/23 16:09:29 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	have_door(t_club *club)
{
	int	i;
	int	j;
	int	have_door;

	i = 0;
	have_door = 0;
	while (club->map.grid[i])
	{
		j = 0;
		while (club->map.grid[i][j])
		{
			if (club->map.grid[i][j] == 'D' || club->map.grid[i][j] == '2' \
				|| club->map.grid[i][j] == '3' || club->map.grid[i][j] == '4' \
				|| club->map.grid[i][j] == '5')
			have_door = 1;
			j++;
		}
		i++;
	}
	return (have_door);
}

int	is_bonus(t_club *club)
{
	int	i;
	int	j;
	int	is_bonus;

	i = 0;
	is_bonus = 0;
	while (club->map.grid[i])
	{
		j = 0;
		while (club->map.grid[i][j])
		{
			if (club->map.grid[i][j] == 'D' || club->map.grid[i][j] == '2' \
				|| club->map.grid[i][j] == '3' || club->map.grid[i][j] == '4' \
				|| club->map.grid[i][j] == '5')
			is_bonus = 1;
			j++;
		}
		i++;
	}
	return (is_bonus);
}

int	init_club(t_club *club)
{
	club->mlx = mlx_init();
	if (!club->mlx)
		return (1);
	club->win = mlx_new_window(club->mlx, WIDTH, HEIGHT, "PINK PONY CLUB");
	if (!club->win)
		return (1);
	if (init_image(club))
		return (1);
	return (0);
}

int	init_bonus(t_club *club)
{
	if (load_win_banner(club))
		return (1);
	if (!init_door_tex_defaults(club))
		return (1);
	if (!init_doors(club))
		return (1);
	if (!init_sprits(club))
		return (1);
	return (0);
}
