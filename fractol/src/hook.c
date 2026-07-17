/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:21:59 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/09 14:11:46 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

static void	reset_fractal(t_fractal *f)
{
	mlx_destroy_image(f->mlx, f->img);
	f->zoom = 1.0;
	f->offset_x = -0.5;
	f->offset_y = 0.0;
	f->max_iter = 100;
	f->color = 0xFCBEEE;
	f->img = mlx_new_image(f->mlx, WIDTH, HEIGHT);
	f->data = mlx_get_data_addr(f->img, &f->bpp, &f->size_line, &f->endian);
	draw_fractal(f);
}

static void	zoom_at(t_fractal *f, int m_x, int m_y, double scale)
{
	double	m_re;
	double	m_im;
	double	new_m_re;
	double	new_m_im;

	m_re = (m_x - WIDTH / 2.0) / (WIDTH / 4.0 * f->zoom) - f->offset_x;
	m_im = (m_y - HEIGHT / 2.0) / (HEIGHT / 4.0 * f->zoom) - f->offset_y;
	f->zoom *= scale;
	new_m_re = (m_x - WIDTH / 2.0) / (WIDTH / 4.0 * f->zoom) - f->offset_x;
	new_m_im = (m_y - HEIGHT / 2.0) / (HEIGHT / 4.0 * f->zoom) - f->offset_y;
	f->offset_x += (new_m_re - m_re);
	f->offset_y += (new_m_im - m_im);
}

int	exit_hook(t_fractal *f)
{
	mlx_destroy_image(f->mlx, f->img);
	mlx_destroy_window(f->mlx, f->win);
	exit(0);
	return (0);
}

int	key_hook(int keycode, t_fractal *f)
{
	if (keycode == ESC)
		exit_hook(f);
	else if (keycode == UP)
		f->offset_y -= 0.2 / f->zoom;
	else if (keycode == DOWN)
		f->offset_y += 0.2 / f->zoom;
	else if (keycode == LEFT)
		f->offset_x -= 0.2 / f->zoom;
	else if (keycode == RIGHT)
		f->offset_x += 0.2 / f->zoom;
	else if (keycode == 'R' || keycode == 'r')
		reset_fractal(f);
	else if (keycode == 'C' || keycode == 'c')
		f->color += 0x001111;
	else if (keycode == '+' || keycode == '=')
		change_iterations(f, 1);
	else if (keycode == '-')
		change_iterations(f, -1);
	draw_fractal(f);
	return (0);
}

int	mouse_hook(int button, int x, int y, t_fractal *f)
{
	if (button == SCROLL_UP)
		zoom_at(f, x, y, 1.1);
	else if (button == SCROLL_DOWN)
		zoom_at(f, x, y, 0.9);
	draw_fractal(f);
	return (0);
}
