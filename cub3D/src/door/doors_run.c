/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_run.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:26:25 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:26:26 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_door	*door_at(t_club *c, int mx, int my)
{
	int	i;

	i = 0;
	while (i < c->door_count)
	{
		if ((int)c->doors[i].x == mx && (int)c->doors[i].y == my)
			return (&c->doors[i]);
		i++;
	}
	return (NULL);
}

void	try_open_door(t_club *c)
{
	double	fx;
	double	fy;
	int		mx;
	int		my;
	t_door	*d;

	fx = c->player.x + c->player.dir_x * 1.8;
	fy = c->player.y + c->player.dir_y * 1.8;
	mx = (int)fx;
	my = (int)fy;
	if (my < 0 || my >= c->map.height || mx < 0 || mx >= c->map.width)
		return ;
	if (c->map.grid[my][mx] != 'D')
		return ;
	d = door_at(c, mx, my);
	if (!d)
		return ;
	if (d->state == DOOR_CLOSED)
		d->state = DOOR_OPENING;
	else if (d->state == DOOR_OPEN)
		d->state = DOOR_CLOSING;
}

static void	door_opening_step(t_door *d, double speed)
{
	d->t += speed;
	if (d->t >= 1.0)
	{
		d->t = 1.0;
		d->state = DOOR_OPEN;
	}
}

static void	door_closing_step(t_door *d, double speed)
{
	d->t -= speed;
	if (d->t <= 0.0)
	{
		d->t = 0.0;
		d->state = DOOR_CLOSED;
	}
}

void	update_doors(t_club *c)
{
	int		i;
	double	speed;

	speed = 0.08;
	i = 0;
	while (i < c->door_count)
	{
		if (c->doors[i].state == DOOR_OPENING)
			door_opening_step(&c->doors[i], speed);
		else if (c->doors[i].state == DOOR_CLOSING)
			door_closing_step(&c->doors[i], speed);
		i++;
	}
}
