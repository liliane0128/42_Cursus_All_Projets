/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_init_step.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:27:33 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:27:34 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	init_delta(t_step *s, t_ray *ray)
{
	if (ray->ray_dir_x == 0)
		s->delta_x = 1e30;
	else
		s->delta_x = fabs(1.0 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		s->delta_y = 1e30;
	else
		s->delta_y = fabs(1.0 / ray->ray_dir_y);
}

static void	init_step_x(t_club *club, t_step *s, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		s->step_x = -1;
		s->dist_x = (club->player.x - s->map_x) * s->delta_x;
	}
	else
	{
		s->step_x = 1;
		s->dist_x = (s->map_x + 1.0 - club->player.x) * s->delta_x;
	}
}

static void	init_step_y(t_club *club, t_step *s, t_ray *ray)
{
	if (ray->ray_dir_y < 0)
	{
		s->step_y = -1;
		s->dist_y = (club->player.y - s->map_y) * s->delta_y;
	}
	else
	{
		s->step_y = 1;
		s->dist_y = (s->map_y + 1.0 - club->player.y) * s->delta_y;
	}
}

void	init_step(t_club *club, t_ray *ray, t_step *s)
{
	s->map_x = (int)club->player.x;
	s->map_y = (int)club->player.y;
	init_delta(s, ray);
	init_step_x(club, s, ray);
	init_step_y(club, s, ray);
}
