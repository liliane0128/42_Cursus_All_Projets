/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:08:03 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/09 13:14:16 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "libft/libft.h"
# include "minilibx/mlx.h"
# include <math.h>

# define WIDTH 800
# define HEIGHT 800

// Linux keycodes
# define ESC 65307
# define UP 65362
# define DOWN 65364
# define LEFT 65361
# define RIGHT 65363
# define SCROLL_UP 4
# define SCROLL_DOWN 5

typedef struct s_fractal
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*data;
	int		bpp;
	int		size_line;
	int		endian;
	char	*name;
	int		x;
	int		y;
	double	zx;
	double	zy;
	double	cx;
	double	cy;
	double	zoom;
	double	offset_x;
	double	offset_y;
	int		color;
	double	julia_re;
	double	julia_im;
	int		max_iter;
}	t_fractal;

/* hook.c*/
int		exit_hook(t_fractal *f);
int		key_hook(int keycode, t_fractal *f);
int		mouse_hook(int button, int x, int y, t_fractal *f);

/*draw.c*/
void	put_color_to_pixel(t_fractal *f, int x, int y, int color);
void	draw_fractal(t_fractal *f);

/*fractals*/
void	julia(t_fractal *f);
void	mandelbrot(t_fractal *f);
void	tricorn(t_fractal *f);

/*util.c*/
double	ft_atof(const char *s);
int		is_valid_float(const char *s);
void	change_iterations(t_fractal *f, int delta);

/*init.c*/
void	init(t_fractal *f);

#endif
