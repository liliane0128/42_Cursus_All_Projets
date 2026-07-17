/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_collectibles.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:47 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:28:48 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	mark_sprite_found(t_club *club, t_sprite *s)
{
	s->found = true;
	club->found_count++;
	if (club->found_count == club->sprite_count)
		club->game_won = 1;
}

static int	sprite_is_collected(t_club *club, t_sprite *s)
{
	double	dx;
	double	dy;
	double	r;
	double	pr;

	pr = 0.25;
	dx = club->player.x - s->x;
	dy = club->player.y - s->y;
	r = pr + s->radius + 0.05;
	if (dx * dx + dy * dy <= r * r)
		return (1);
	return (0);
}

void	update_collectibles(t_club *club)
{
	int			i;
	t_sprite	*s;

	if (!club->sprites || club->sprite_count <= 0 || club->game_won)
		return ;
	i = 0;
	while (i < club->sprite_count)
	{
		s = &club->sprites[i];
		if (!s->found && sprite_is_collected(club, s))
			mark_sprite_found(club, s);
		i++;
	}
}

// void	update_collectibles(t_club *club)
// {
// 	int				i;
// 	t_sprite		*s;
// 	double			dx;
// 	double			dy;
// 	double			r;

// 	if (!club->sprites || club->sprite_count <= 0 || club->game_won)
// 		return ;
// 	i = 0;
// 	while (i < club->sprite_count)
// 	{
// 		s = &club->sprites[i];
// 		if (!s->found)
// 		{
// 			dx = club->player.x - s->x;
// 			dy = club->player.y - s->y;
// 			const double	pr = 0.25;
// 			r = pr + s->radius + 0.05;
// 			if (dx * dx + dy * dy <= r * r)
// 			{
// 				s->found = true;
// 				club->found_count++;
// 				if (club->found_count == club->sprite_count)
// 					club->game_won = 1;
// 			}
// 		}
// 		i++;
// 	}
// }
