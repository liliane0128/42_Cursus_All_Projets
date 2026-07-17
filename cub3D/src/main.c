/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:26:52 by hguo              #+#    #+#             */
/*   Updated: 2026/01/26 19:11:06 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	parse(t_club *club, char *path)
{
	club->file = read_file(path);
	if (!club->file)
		return (1);
	if (parsing(club, club->file) == -1)
	{
		free_array(club->file);
		club->file = NULL;
		return (1);
	}
	return (0);
}

static int	run_game(t_club *club, bool bonus)
{
	if (load_all_textures(club))
		return (1);
	if (bonus)
	{
		if (load_sprites(club))
			return (1);
	}
	mlx_hook(club->win, 17, 0, close_window, club);
	mlx_key_hook(club->win, key_hook, club);
	mlx_hook(club->win, 4, 1L << 2, mouse_press, club);
	mlx_hook(club->win, 5, 1L << 3, mouse_release, club);
	mlx_hook(club->win, 6, 1L << 6, mouse_move, club);
	mlx_hook(club->win, 2, 1L << 0, key_press, club);
	mlx_hook(club->win, 3, 1L << 1, key_release, club);
	mlx_loop_hook(club->mlx, render_loop, club);
	mlx_loop(club->mlx);
	return (0);
}

int	main(int argc, char **argv)
{
	t_club	club;
	int		bonus;

	if (argc != 2)
		return (err_msg("Usage: ./cub3d <map.cub>"), 1);
	if (!is_cub_file(argv[1]))
		return (err_msg("Error: invalid .cub file"), 1);
	ft_bzero(&club, sizeof(club));
	if (parse(&club, argv[1]))
	{
		destroy_club(&club);
		return (1);
	}
	if (init_club(&club) || init_club_defaults(&club))
		return (destroy_club(&club), 1);
	bonus = is_bonus(&club);
	if (bonus)
	{
		if (init_bonus(&club))
			return (destroy_club(&club), 1);
	}
	if (run_game(&club, bonus))
		return (destroy_club(&club), 1);
	destroy_club(&club);
	return (0);
}
