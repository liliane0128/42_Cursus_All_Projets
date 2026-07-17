/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_door_overlay.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leticiabi <leticiabi@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:26:31 by hguo              #+#    #+#             */
/*   Updated: 2026/01/23 10:06:26 by leticiabi        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	clampi(int v, int lo, int hi)
{
	if (v < lo)
		return (lo);
	if (v > hi)
		return (hi);
	return (v);
}

static int	tex_sample(t_tex *t, int x, int y)
{
	char	*p;

	if (!t || !t->addr)
		return (0);
	x = clampi(x, 0, t->width - 1);
	y = clampi(y, 0, t->height - 1);
	p = t->addr + (y * t->line_len + x * (t->bpp / 8));
	return (*(int *)p);
}

static int	init_door_stripe(t_club *c, t_ray *r, t_tex **tex, t_stripe *st)
{
	*tex = &c->door_open_tex;
	if (r->door_dist < 0.0001 || !(*tex)->addr
		|| (*tex)->width <= 0 || (*tex)->height <= 0)
		return (0);
	st->line_h = (int)(HEIGHT / r->door_dist);
	st->start = -st->line_h / 2 + HEIGHT / 2;
	st->end = st->line_h / 2 + HEIGHT / 2;
	if (st->start < 0)
		st->start = 0;
	if (st->end >= HEIGHT)
		st->end = HEIGHT - 1;
	st->tex_x = (int)(r->door_hit_point * (double)(*tex)->width);
	if (st->tex_x < 0)
		st->tex_x = 0;
	if (st->tex_x >= (*tex)->width)
		st->tex_x = (*tex)->width - 1;
	if (r->door_side == 0 && r->ray_dir_x > 0)
		st->tex_x = (*tex)->width - st->tex_x - 1;
	if (r->door_side == 1 && r->ray_dir_y < 0)
		st->tex_x = (*tex)->width - st->tex_x - 1;
	st->step = (double)(*tex)->height / (double)st->line_h;
	st->tex_pos = (st->start - HEIGHT / 2 + st->line_h / 2) * st->step;
	return (1);
}

void	draw_door_overlay(t_club *club, int x, t_ray *ray)
{
	t_tex			*tex;
	t_stripe		st;
	int				y;
	int				tex_y;
	int				color;

	if (!init_door_stripe(club, ray, &tex, &st))
		return ;
	y = st.start;
	while (y <= st.end)
	{
		tex_y = (int)st.tex_pos;
		color = tex_sample(tex, st.tex_x, tex_y);
		put_pixel(&club->img, x, y, color);
		st.tex_pos += st.step;
		y++;
	}
}
