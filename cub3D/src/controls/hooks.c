/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:26:39 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:26:40 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	close_window(t_club *club)
{
	if (!club)
		exit(0);
	destroy_club(club);
	if (club->file)
	{
		free_array(club->file);
		club->file = NULL;
	}
	if (club->mlx)
	{
		mlx_destroy_display(club->mlx);
		free(club->mlx);
		club->mlx = NULL;
	}
	exit(0);
	return (0);
}

static int	handle_move_keys(int keycode, t_club *club, double move_speed)
{
	if (keycode == KEY_W)
		move_forward_backward(club, move_speed);
	else if (keycode == KEY_S)
		move_forward_backward(club, -move_speed);
	else if (keycode == KEY_A)
		strafe_left_right(club, -move_speed);
	else if (keycode == KEY_D)
		strafe_left_right(club, move_speed);
	else
		return (0);
	return (1);
}

static int	handle_rot_keys(int keycode, t_club *club, double rot_speed)
{
	if (keycode == KEY_LEFT)
		rotate_player(club, -rot_speed);
	else if (keycode == KEY_RIGHT)
		rotate_player(club, rot_speed);
	else
		return (0);
	return (1);
}

static void	update_mouse_sens(t_club *club, double factor,
	double min, double max)
{
	club->mouse_sens *= factor;
	if (club->mouse_sens < min)
		club->mouse_sens = min;
	if (club->mouse_sens > max)
		club->mouse_sens = max;
}

int	key_hook(int keycode, t_club *club)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 0.1;
	rot_speed = 0.09;
	if (keycode == KEY_ESC)
		close_window(club);
	else if (handle_move_keys(keycode, club, move_speed))
		;
	else if (handle_rot_keys(keycode, club, rot_speed))
		;
	else if (keycode == KEY_O)
		try_open_door(club);
	else if (keycode == KEY_MAP)
		club->show_minimap = !club->show_minimap;
	else if (keycode == KEY_Q)
		update_mouse_sens(club, 0.8, 0.0005, 0.05);
	else if (keycode == KEY_E)
		update_mouse_sens(club, 1.25, 0.0005, 0.05);
	return (0);
}
