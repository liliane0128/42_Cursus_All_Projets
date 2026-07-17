/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:27 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/09 13:00:24 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void	mandelbrot(t_fractal *f)
{
	int		i;
	double	zx_temp;

	f->zx = 0.0;
	f->zy = 0.0;
	f->cx = (f->x - WIDTH / 2.0) / (WIDTH / 4.0 * f->zoom) - f->offset_x;
	f->cy = (f->y - HEIGHT / 2.0) / (HEIGHT / 4.0 * f->zoom) - f->offset_y;
	i = -1;
	while (++i < f->max_iter)
	{
		zx_temp = f->zx * f->zx - f->zy * f->zy + f->cx;
		f->zy = 2.0 * f->zx * f->zy + f->cy;
		f->zx = zx_temp;
		if (f->zx * f->zx + f->zy * f->zy > 4.0)
			break ;
	}
	if (i == f->max_iter)
		put_color_to_pixel(f, f->x, f->y, 0x000000);
	else
		put_color_to_pixel(f, f->x, f->y, f->color * i);
}
