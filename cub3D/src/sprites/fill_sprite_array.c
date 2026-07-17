/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_sprite_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:26 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:30:27 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	sprite_type_from_char(char c)
{
	if (c == '2')
		return (SPR_PONY);
	if (c == '3')
		return (SPR_SPARKLE);
	if (c == '4')
		return (SPR_HEART);
	if (c == '5')
		return (SPR_DISCO);
	return (-1);
}

static void	init_sprite_params(t_sprite *s, int x, int y)
{
	s->radius = 0.6;
	s->visible = false;
	s->distance = 0;
	s->phase = (y * 7 + x * 13) * 0.37;
	s->found = false;
}

bool	fill_sprite_array(t_club *club)
{
	int	y;
	int	x;
	int	idx;
	int	type;

	y = -1;
	idx = 0;
	while (club->map.grid[++y] && idx < club->sprite_count)
	{
		x = -1;
		while (club->map.grid[y][++x] && idx < club->sprite_count)
		{
			type = sprite_type_from_char(club->map.grid[y][x]);
			if (type == -1)
				continue ;
			club->sprites[idx].x = x + 0.5;
			club->sprites[idx].y = y + 0.5;
			club->sprites[idx].type = type;
			init_sprite_params(&club->sprites[idx], x, y);
			club->map.grid[y][x] = '0';
			idx++;
		}
	}
	return (true);
}
