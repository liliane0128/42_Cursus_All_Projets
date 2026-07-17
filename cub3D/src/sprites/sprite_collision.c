/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:57 by hguo              #+#    #+#             */
/*   Updated: 2026/01/27 17:33:04 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	is_sprite_char(char c)
{
	return (c == '2' || c == '3' || c == '4' || c == '5');
}

int	count_sprites_all(char **map)
{
	int	x;
	int	y;
	int	count;

	if (!map)
		return (0);
	x = 0;
	count = 0;
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (is_sprite_char(map[y][x]))
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

/*
** Detects sprite collision using a 2D circle-to-point distance check.
** pr = 0.25 represents the player collision radius, sprite->radius defines the
** sprite hitbox size.
** The extra 0.04 is a safety margin to prevent the player from
** passing through sprites due to floating-point errors.
*/
int	collide_sprite_at(t_club *club, double nx, double ny)
{
	t_sprite		*s;
	double			d[3];
	const double	pr = 0.1;
	int				i;

	if (!club->sprites || club->sprite_count <= 0)
		return (0);
	i = 0;
	while (i < club->sprite_count)
	{
		s = &club->sprites[i];
		d[0] = nx - s->x;
		d[1] = ny - s->y;
		d[2] = pr + s->radius + 0.04;
		if (d[0] * d[0] + d[1] * d[1] < d[2] * d[2])
			return (1);
		i++;
	}
	return (0);
}
