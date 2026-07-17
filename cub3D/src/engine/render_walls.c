/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:26 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:28:27 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	set_ray_dir(t_club *club, t_ray *ray, int x)
{
	double	camera_x;

	camera_x = 2.0 * x / (double)WIDTH - 1.0;
	ray->ray_dir_x = club->player.dir_x + club->player.plane_x * camera_x;
	ray->ray_dir_y = club->player.dir_y + club->player.plane_y * camera_x;
}

static void	init_ray_defaults(t_ray *ray)
{
	ray->has_door = 0;
	ray->door_dist = 0;
}

static void	draw_wall(t_club *club, int x, t_ray *ray)
{
	if (ray->hit_type == HIT_WALL || ray->hit_type == HIT_DOOR)
		draw_wall_stripe(club, x, ray);
}

void	render_walls(t_club *club)
{
	int		x;
	t_ray	ray;
	t_step	s;

	x = 0;
	while (x < WIDTH)
	{
		init_ray_defaults(&ray);
		set_ray_dir(club, &ray, x);
		init_step(club, &ray, &s);
		walk_until_wall(club, &s, &ray);
		compute_wall(&ray, &s);
		club->z_buffer[x] = ray.perp_wall_dist;
		draw_wall(club, x, &ray);
		x++;
	}
}
