/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:06 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 18:12:49 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	mm_tile_px(void)
{
	int	t;

	t = MM_W / (2 * MM_R + 1);
	if (t < 4)
		t = 4;
	return (t);
}

static void	mm_clear(t_club *club)
{
	int	x;
	int	y;

	y = 0;
	while (y < MM_H)
	{
		x = 0;
		while (x < MM_W)
		{
			put_pixel(&club->img, MM_ORIGIN_X + x, MM_ORIGIN_Y + y, MM_BG);
			x++;
		}
		y++;
	}
}

static void	mm_draw_tiles(t_club *club, int px, int py, int tile)
{
	t_mm_draw	d;
	int			tx;
	int			ty;
	int			start_x;
	int			start_y;

	d.img = &club->img;
	d.size = tile;
	start_x = px - MM_R;
	start_y = py - MM_R;
	ty = start_y;
	while (ty <= py + MM_R)
	{
		tx = start_x;
		while (tx <= px + MM_R)
		{
			d.x = MM_ORIGIN_X + (tx - start_x) * tile;
			d.y = MM_ORIGIN_Y + (ty - start_y) * tile;
			d.color = mm_tile_color(club, tx, ty);
			mm_draw_square(&d);
			tx++;
		}
		ty++;
	}
}

static void	mm_draw_sprites_in_view(t_club *club, int tile)
{
	int			i;
	double		dx;
	double		dy;
	t_mm_draw	d;

	d.img = &club->img;
	d.size = 0;
	d.color = 0;
	i = 0;
	while (i < club->sprite_count)
	{
		if (!club->sprites[i].found)
		{
			dx = club->sprites[i].x - club->player.x;
			dy = club->sprites[i].y - club->player.y;
			d.x = MM_ORIGIN_X + MM_R * tile + (int)(dx * tile);
			d.y = MM_ORIGIN_Y + MM_R * tile + (int)(dy * tile);
			if (d.x >= MM_ORIGIN_X && d.x < MM_ORIGIN_X + MM_W
				&& d.y >= MM_ORIGIN_Y && d.y < MM_ORIGIN_Y + MM_H)
				mm_draw_sprite(&d, tile);
		}
		i++;
	}
}

void	render_minimap(t_club *club)
{
	int			tile;
	int			px;
	int			py;
	t_mm_draw	d;

	tile = mm_tile_px();
	px = (int)club->player.x;
	py = (int)club->player.y;
	mm_clear(club);
	mm_draw_tiles(club, px, py, tile);
	d.img = &club->img;
	d.size = 0;
	d.color = 0;
	d.x = MM_ORIGIN_X + MM_R * tile + (int)((club->player.x - px) * tile);
	d.y = MM_ORIGIN_Y + MM_R * tile + (int)((club->player.y - py) * tile);
	mm_draw_player_pony(&d, tile);
	mm_draw_sprites_in_view(club, tile);
}
