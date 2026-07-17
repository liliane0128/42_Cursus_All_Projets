/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:50:03 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/07 13:37:42 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

static int	error_exit(void)
{
	ft_putstr_fd("Error: invalid arguments.\n", 2);
	ft_putstr_fd("Usage:\n", 2);
	ft_putstr_fd("./fractol mandelbrot\n", 2);
	ft_putstr_fd("./fractol tricorn\n", 2);
	ft_putstr_fd("./fractol julia <re> <im>\n", 2);
	return (1);
}

static int	error_julia_exit(void)
{
	ft_putstr_fd("Error: Julia parametres must be valid floats ", 2);
	ft_putstr_fd("between -2.0 and 2.0.\n", 2);
	return (1);
}

static int	check_julia_args(int ac, char **av, t_fractal *f)
{
	if (ac == 4)
	{
		if (!is_valid_float(av[2]) || !is_valid_float(av[3]))
			return (error_julia_exit());
		f->julia_re = ft_atof(av[2]);
		f->julia_im = ft_atof(av[3]);
		if (f->julia_re < -2.0 || f->julia_re > 2.0
			|| f->julia_im < -2.0 || f->julia_im > 2.0)
			return (error_julia_exit());
	}
	else if (ac == 2)
	{
		f->julia_re = -0.745429;
		f->julia_im = 0.05;
	}
	return (0);
}

static int	check_args(int ac, char **av, t_fractal *f)
{
	if (ac != 2 && ac != 4)
		return (error_exit());
	if (ft_strcmp(av[1], "julia") == 0)
	{
		f->name = "julia";
		return (check_julia_args(ac, av, f));
	}
	else if (ac == 2 && ft_strcmp(av[1], "mandelbrot") == 0)
			f->name = "mandelbrot";
	else if (ac == 2 && ft_strcmp(av[1], "tricorn") == 0)
			f->name = "tricorn";
	else
		return (error_exit());
	return (0);
}

int	main(int ac, char **av)
{
	t_fractal	f;

	if (check_args(ac, av, &f))
		return (1);
	init(&f);
	draw_fractal(&f);
	mlx_key_hook(f.win, key_hook, &f);
	mlx_mouse_hook(f.win, mouse_hook, &f);
	mlx_hook(f.win, 17, 0, exit_hook, &f);
	mlx_loop(f.mlx);
	return (0);
}

/*
in main: 
mlx_key_hook: Handle key press events (like arrows, ESC, etc.)
mlx_mouse_hook: Handle mouse click events (like zooming with scroll)
mlx_hook: Handle window close event (when user clicks the red X)
		17 is the X11 event code, indicating that the window is closed.
		0 is the event mask parameter, here, 0 means "no specific mask".
mlx_loop: Start the main loop to keep window open and listen for events
X11 :  a window system used on Unix/Linux that manages graphical displays 
	and user input. And MiniLibX is a simple graphics library that uses X11.
*/