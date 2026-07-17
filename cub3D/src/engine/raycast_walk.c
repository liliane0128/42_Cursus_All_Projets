/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_walk.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:27:38 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:27:39 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	step_dda(t_step *s)
{
	if (s->dist_x < s->dist_y)
	{
		s->dist_x += s->delta_x;
		s->map_x += s->step_x;
		s->side = 0;
	}
	else
	{
		s->dist_y += s->delta_y;
		s->map_y += s->step_y;
		s->side = 1;
	}
}

static int	check_hit_cell(t_club *club, t_step *s, t_ray *ray)
{
	char	cell;

	cell = club->map.grid[s->map_y][s->map_x];
	if (cell == '1')
	{
		ray->hit_type = HIT_WALL;
		ray->side = s->side;
		return (1);
	}
	if (cell == 'D')
		return (handle_door_cell(club, s, ray));
	return (0);
}

void	walk_until_wall(t_club *club, t_step *s, t_ray *ray)
{
	int	hit;

	hit = 0;
	ray->hit_type = HIT_NONE;
	while (!hit)
	{
		step_dda(s);
		hit = check_hit_cell(club, s, ray);
	}
}

static double	get_wall_dist(t_step *s)
{
	double	dist;

	if (s->side == 0)
		dist = s->dist_x - s->delta_x;
	else
		dist = s->dist_y - s->delta_y;
	if (dist < 0.0001)
		dist = 0.0001;
	return (dist);
}

void	compute_wall(t_ray *ray, t_step *s)
{
	double	dist;

	dist = get_wall_dist(s);
	ray->perp_wall_dist = dist;
	ray->line_height = (int)(HEIGHT / dist);
	ray->draw_start = -ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_end >= HEIGHT)
		ray->draw_end = HEIGHT - 1;
	ray->side = s->side;
}
