/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_stripe_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:52 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:28:53 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	choose_texture(t_ray *r)
{
	if (r->side == 0)
	{
		if (r->ray_dir_x > 0)
			return (TEX_WE);
		return (TEX_EA);
	}
	if (r->ray_dir_y > 0)
		return (TEX_NO);
	return (TEX_SO);
}

void	select_stripe_tex(t_club *club, t_ray *r, t_stripe *st)
{
	int	tex_id;

	st->tex = NULL;
	if (r->hit_type == HIT_DOOR)
	{
		st->tex = &club->door_tex;
		return ;
	}
	tex_id = choose_texture(r);
	st->tex = &club->tex[tex_id];
}

int	compute_stripe_tex_x(t_club *club, t_ray *r, t_tex *tex)
{
	double	wall_x;
	int		tex_x;

	if (r->side == 0)
		wall_x = club->player.y + r->perp_wall_dist * r->ray_dir_y;
	else
		wall_x = club->player.x + r->perp_wall_dist * r->ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)tex->width);
	if (r->side == 0 && r->ray_dir_x > 0)
		tex_x = tex->width - tex_x - 1;
	if (r->side == 1 && r->ray_dir_y < 0)
		tex_x = tex->width - tex_x - 1;
	return (tex_x);
}

void	apply_door_shift(t_club *club, t_ray *r, t_stripe *st)
{
	t_door	*d;
	int		shift;

	if (r->hit_type != HIT_DOOR)
		return ;
	d = door_at(club, r->hit_map_x, r->hit_map_y);
	if (!d)
		return ;
	shift = (int)(d->t * (double)st->tex->width);
	st->tex_x += shift;
}

void	draw_stripe_column(t_club *club, int x, t_ray *r, t_stripe *st)
{
	int		tex_y;
	char	*pixel;
	int		color;

	while (st->y < r->draw_end)
	{
		tex_y = (int)st->tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= st->tex->height)
			tex_y = st->tex->height - 1;
		st->tex_pos += st->step;
		if (r->hit_type == HIT_DOOR && (st->tex_x < 0
				|| st->tex_x >= st->tex->width))
		{
			st->y++;
			continue ;
		}
		pixel = st->tex->addr + tex_y * st->tex->line_len
			+ st->tex_x * (st->tex->bpp / 8);
		color = *(int *)pixel;
		put_pixel(&club->img, x, st->y, color);
		st->y++;
	}
}
