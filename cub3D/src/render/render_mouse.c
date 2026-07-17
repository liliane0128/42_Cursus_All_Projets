/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:10 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:28:11 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	mouse_press(int button, int x, int y, t_club *club)
{
	(void)x;
	(void)y;
	if (button == 1)
		club->mouse_left = 1;
	return (0);
}

int	mouse_release(int button, int x, int y, t_club *club)
{
	(void)x;
	(void)y;
	if (button == 1)
		club->mouse_left = 0;
	return (0);
}

static int	mouse_can_rotate(int x, t_club *club)
{
	if (!club->mouse_left)
	{
		club->mouse_in_window = 0;
		return (0);
	}
	if (x < 0 || x >= WIDTH)
	{
		club->mouse_in_window = 0;
		return (0);
	}
	if (!club->mouse_in_window)
	{
		club->mouse_last_x = x;
		club->mouse_in_window = 1;
		return (0);
	}
	return (1);
}

int	mouse_move(int x, int y, t_club *club)
{
	int		dx;
	double	rot_speed;

	(void)y;
	if (!mouse_can_rotate(x, club))
		return (0);
	dx = x - club->mouse_last_x;
	if (dx == 0)
		return (0);
	rot_speed = dx * club->mouse_sens;
	rotate_player(club, -rot_speed);
	club->mouse_last_x = x;
	return (0);
}
