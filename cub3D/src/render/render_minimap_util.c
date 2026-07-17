/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap_util.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:02 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 18:13:08 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	mm_tile_color(t_club *club, int tx, int ty)
{
	if (ty < 0 || ty >= club->map.height || tx < 0 || tx >= club->map.width)
		return (MM_BG);
	return (mm_color_for_cell(club->map.grid[ty][tx]));
}

int	mm_color_for_cell(char c)
{
	if (c == '1')
		return (MM_WALL);
	if (c == 'D')
		return (MM_DOOR);
	return (MM_BG);
}

void	mm_draw_square(t_mm_draw *d)
{
	int	sx;
	int	sy;

	sy = 0;
	while (sy < d->size)
	{
		sx = 0;
		while (sx < d->size)
		{
			put_pixel(d->img, d->x + sx, d->y + sy, d->color);
			sx++;
		}
		sy++;
	}
}

void	mm_draw_player_pony(t_mm_draw *d, int tile)
{
	int	radius;
	int	r2;
	int	x;
	int	y;

	radius = tile / 3;
	if (radius < 3)
		radius = 3;
	r2 = radius * radius;
	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= r2)
				put_pixel(d->img, d->x + x, d->y + y, MM_PLAYER);
			x++;
		}
		y++;
	}
}

void	mm_draw_sprite(t_mm_draw *d, int tile)
{
	int	radius;
	int	r2;
	int	x;
	int	y;

	radius = tile / 4;
	if (radius < 2)
		radius = 2;
	r2 = radius * radius;
	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= r2)
				put_pixel(d->img, d->x + x, d->y + y, MM_SPRITE);
			x++;
		}
		y++;
	}
}
