/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:27:57 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:27:58 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	reset_z_buffer(t_club *club)
{
	int	x;

	x = 0;
	while (x < WIDTH)
	{
		club->z_buffer[x] = 1e30;
		x++;
	}
}

static void	draw_scene(t_club *club)
{
	render_background(club);
	render_walls(club);
	render_sprites(club);
	if (club->show_minimap)
		render_minimap(club);
	render_score_ui(club);
	mlx_put_image_to_window(club->mlx, club->win, club->img.img, 0, 0);
}

static void	draw_win_overlay(t_club *club)
{
	int	x;
	int	y;

	if (!club->game_won)
		return ;
	if (club->win_img.img)
	{
		x = (WIDTH - club->win_img.width) / 2;
		y = (HEIGHT - club->win_img.height) / 2;
		mlx_put_image_to_window(club->mlx, club->win,
			club->win_img.img, x, y);
		return ;
	}
	mlx_string_put(club->mlx, club->win, WIDTH / 2 - 40,
		HEIGHT / 2, 0xFFFFFF, "YOU WIN!");
}

int	render_loop(t_club *club)
{
	club->sprite_jump++;
	update_doors(club);
	update_player(club);
	update_collectibles(club);
	reset_z_buffer(club);
	draw_scene(club);
	draw_win_overlay(club);
	return (0);
}
