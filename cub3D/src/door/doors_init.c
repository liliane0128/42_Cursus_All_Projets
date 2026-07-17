/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:26:07 by hguo              #+#    #+#             */
/*   Updated: 2026/01/23 14:22:37 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	save_door_hit(t_club *c, t_step *s, t_ray *r)
{
	double	dist;
	double	hp;

	if (r->has_door)
		return ;
	if (s->side == 0)
		dist = (s->map_x - c->player.x
				+ (1 - s->step_x) / 2.0) / r->ray_dir_x;
	else
		dist = (s->map_y - c->player.y
				+ (1 - s->step_y) / 2.0) / r->ray_dir_y;
	if (dist <= 0.0001)
		return ;
	r->has_door = 1;
	r->door_dist = dist;
	r->door_side = s->side;
	if (s->side == 0)
		hp = c->player.y + dist * r->ray_dir_y;
	else
		hp = c->player.x + dist * r->ray_dir_x;
	r->door_hit_point = hp - floor(hp);
}

int	handle_door_cell(t_club *c, t_step *s, t_ray *r)
{
	t_door	*d;

	save_door_hit(c, s, r);
	d = door_at(c, s->map_x, s->map_y);
	if (d && d->state == DOOR_OPEN)
		return (0);
	r->hit_type = HIT_DOOR;
	r->side = s->side;
	r->hit_map_x = s->map_x;
	r->hit_map_y = s->map_y;
	return (1);
}

static void	init_single_door(t_club *club, int x, int y, int *i)
{
	club->doors[*i].x = x;
	club->doors[*i].y = y;
	club->doors[*i].state = DOOR_CLOSED;
	club->doors[*i].t = 0.0;
	(*i)++;
}

static void	scan_door_row(t_club *club, int y, int *i)
{
	int	x;

	x = 0;
	while (x < club->map.width)
	{
		if (club->map.grid[y][x] == 'D')
			init_single_door(club, x, y, i);
		x++;
	}
}

bool	init_doors(t_club *club)
{
	int	y;
	int	i;

	club->door_count = count_char_in_map(club->map.grid, 'D');
	if (club->door_count <= 0)
		return (true);
	club->doors = ft_calloc(club->door_count, sizeof(t_door));
	if (!club->doors)
		return (false);
	i = 0;
	y = 0;
	while (y < club->map.height)
	{
		scan_door_row(club, y, &i);
		y++;
	}
	return (true);
}
