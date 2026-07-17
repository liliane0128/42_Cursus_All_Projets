/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:26:35 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:40:35 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	handle_misc_keys(int keycode, t_club *c)
{
	if (keycode == KEY_O)
		try_open_door(c);
	else if (keycode == KEY_MAP)
		c->show_minimap = !c->show_minimap;
	else if (keycode == KEY_Q)
	{
		c->mouse_sens *= 0.8;
		if (c->mouse_sens < 0.0005)
			c->mouse_sens = 0.0005;
	}
	else if (keycode == KEY_E)
	{
		c->mouse_sens *= 1.25;
		if (c->mouse_sens > 0.05)
			c->mouse_sens = 0.05;
	}
}

int	key_press(int keycode, t_club *c)
{
	if (keycode == KEY_ESC)
		close_window(c);
	else if (keycode == KEY_W)
		c->key_w = 1;
	else if (keycode == KEY_S)
		c->key_s = 1;
	else if (keycode == KEY_A)
		c->key_a = 1;
	else if (keycode == KEY_D)
		c->key_d = 1;
	else if (keycode == KEY_LEFT)
		c->key_left = 1;
	else if (keycode == KEY_RIGHT)
		c->key_right = 1;
	else
		handle_misc_keys(keycode, c);
	return (0);
}

int	key_release(int keycode, t_club *c)
{
	if (keycode == KEY_W)
		c->key_w = 0;
	else if (keycode == KEY_S)
		c->key_s = 0;
	else if (keycode == KEY_A)
		c->key_a = 0;
	else if (keycode == KEY_D)
		c->key_d = 0;
	else if (keycode == KEY_LEFT)
		c->key_left = 0;
	else if (keycode == KEY_RIGHT)
		c->key_right = 0;
	return (0);
}

void	update_player(t_club *c)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 0.03;
	rot_speed = 0.03;
	if (c->key_w)
		move_forward_backward(c, move_speed);
	if (c->key_s)
		move_forward_backward(c, -move_speed);
	if (c->key_a)
		strafe_left_right(c, -move_speed);
	if (c->key_d)
		strafe_left_right(c, move_speed);
	if (c->key_left)
		rotate_player(c, -rot_speed);
	if (c->key_right)
		rotate_player(c, rot_speed);
}
