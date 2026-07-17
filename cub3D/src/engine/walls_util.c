/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:29:05 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:29:06 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	draw_wall_stripe(t_club *club, int x, t_ray *r)
{
	t_stripe	st;

	if (r->line_height <= 0)
		return ;
	if (r->draw_start < 0)
		r->draw_start = 0;
	if (r->draw_end >= HEIGHT)
		r->draw_end = HEIGHT - 1;
	select_stripe_tex(club, r, &st);
	if (!st.tex || !st.tex->img || !st.tex->addr
		|| st.tex->width <= 0 || st.tex->height <= 0)
		return ;
	st.tex_x = compute_stripe_tex_x(club, r, st.tex);
	apply_door_shift(club, r, &st);
	st.step = 1.0 * st.tex->height / r->line_height;
	st.tex_pos = (r->draw_start - HEIGHT / 2 + r->line_height / 2) * st.step;
	st.y = r->draw_start;
	draw_stripe_column(club, x, r, &st);
}

// 判断某个格子是不是墙
int	is_wall(t_club *club, int map_x, int map_y)
{
	char	cell;
	t_door	*d;

	if (map_x < 0 || map_x >= club->map.width
		|| map_y < 0 || map_y >= club->map.height)
		return (1);
	cell = club->map.grid[map_y][map_x];
	if (cell == '1')
		return (1);
	if (cell == 'D')
	{
		d = door_at(club, map_x, map_y);
		if (!d)
			return (0);
		if (d->state != DOOR_OPEN)
			return (1);
	}
	return (0);
}
