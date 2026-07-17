/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:27:29 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:27:30 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	move_forward_backward(t_club *club, double move_speed)
{
	double	new_x;
	double	new_y;

	new_x = club->player.x + club->player.dir_x * move_speed;
	new_y = club->player.y + club->player.dir_y * move_speed;
	if (!is_wall(club, (int)new_x, (int)club->player.y)
		&& !collide_sprite_at(club, new_x, club->player.y))
		club->player.x = new_x;
	if (!is_wall(club, (int)club->player.x, (int)new_y)
		&& !collide_sprite_at(club, club->player.x, new_y))
		club->player.y = new_y;
}

void	strafe_left_right(t_club *club, double move_speed)
{
	double	perp_x;
	double	perp_y;
	double	new_x;
	double	new_y;

	perp_x = -club->player.dir_y;
	perp_y = club->player.dir_x;
	new_x = club->player.x + perp_x * move_speed;
	new_y = club->player.y + perp_y * move_speed;
	if (!is_wall(club, (int)new_x, (int)club->player.y)
		&& !collide_sprite_at(club, new_x, club->player.y))
		club->player.x = new_x;
	if (!is_wall(club, (int)club->player.x, (int)new_y)
		&& !collide_sprite_at(club, club->player.x, new_y))
		club->player.y = new_y;
}

void	rotate_player(t_club *club, double rot_speed)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = club->player.dir_x;
	old_plane_x = club->player.plane_x;
	club->player.dir_x = club->player.dir_x * cos(rot_speed)
		- club->player.dir_y * sin(rot_speed);
	club->player.dir_y = old_dir_x * sin(rot_speed)
		+ club->player.dir_y * cos(rot_speed);
	club->player.plane_x = club->player.plane_x * cos(rot_speed)
		- club->player.plane_y * sin(rot_speed);
	club->player.plane_y = old_plane_x * sin(rot_speed)
		+ club->player.plane_y * cos(rot_speed);
}
