/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 16:21:54 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/09 13:34:27 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

void	put_color_to_pixel(t_fractal *f, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = f->data + (y * f->size_line + x * (f->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_fractal(t_fractal *f)
{
	f->y = 0;
	while (f->y < HEIGHT)
	{
		f->x = 0;
		while (f->x < WIDTH)
		{
			if (ft_strcmp(f->name, "julia") == 0)
				julia(f);
			else if (ft_strcmp(f->name, "tricorn") == 0)
				tricorn(f);
			else
				mandelbrot(f);
			f->x++;
		}
		f->y++;
	}
	mlx_put_image_to_window(f->mlx, f->win, f->img, 0, 0);
}

/*
This function below provides an alternative way to generate colors.
To use this coloring scheme in your fractal rendering, simply
replace the third argument of the `put_color_to_pixel`:
put_color_to_pixel(f, f->x, f->y, get_color(i, f->max_iter));

This function creates a color gradient by multiplying the iteration count i
by different constants (7, 5, and 3) for the red, green,and blue channels 
respectively, then taking the remainder modulo 256 to keep values 
within the 0–255 color range. Using different multipliers
ensures the colors cycle at different speeds, producing varied and visually
interesting gradients instead of uniform or repeating colors.


unsigned int get_color(int i, int max_i)
{
	int	r;
	int	g;
	int	b;

	if (i == max_i)
		return (0x000000);
	r = (i * 7) % 256;
	g = (i * 5) % 256;
	b = (i * 3) % 256;
	return (r << 16) | (g << 8) | b;
}
*/