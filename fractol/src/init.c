/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:03:21 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/09 14:12:44 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void	init(t_fractal *f)
{
	f->mlx = mlx_init();
	f->win = mlx_new_window(f->mlx, WIDTH, HEIGHT, f->name);
	f->img = mlx_new_image(f->mlx, WIDTH, HEIGHT);
	f->data = mlx_get_data_addr(f->img, &f->bpp, &f->size_line, &f->endian);
	f->zoom = 1.0;
	f->offset_x = -0.5;
	f->offset_y = 0.0;
	f->max_iter = 80;
	f->color = 0xFCBEEE;
}

/*
 Some color options to use for fractal coloring:
 0xB0E0E6 - Soft blue
 0xFFD700 - Bright orange
*/