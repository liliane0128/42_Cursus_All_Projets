/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:22:31 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/09 12:58:26 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void	julia(t_fractal *f)
{
	int		i;
	double	zx_temp;

	f->zx = (f->x - WIDTH / 2.0) / (WIDTH / 4.0 * f->zoom) - f->offset_x;
	f->zy = (f->y - HEIGHT / 2.0) / (HEIGHT / 4.0 * f->zoom) - f->offset_y;
	i = -1;
	while (++i < f->max_iter)
	{
		zx_temp = f->zx * f->zx - f->zy * f->zy + f->julia_re;
		f->zy = 2.0 * f->zx * f->zy + f->julia_im;
		f->zx = zx_temp;
		if (f->zx * f->zx + f->zy * f->zy > 4.0)
			break ;
	}
	if (i == f->max_iter)
		put_color_to_pixel(f, f->x, f->y, 0x000000);
	else
		put_color_to_pixel(f, f->x, f->y, f->color * i);
}
