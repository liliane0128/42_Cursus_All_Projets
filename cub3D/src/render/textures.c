/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:28:31 by hguo              #+#    #+#             */
/*   Updated: 2026/01/26 15:30:15 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	load_texture(t_club *club, t_tex *tex)
{
	tex->img = mlx_xpm_file_to_image(club->mlx,
			tex->path, &tex->width, &tex->height);
	if (!tex->img)
		return (1);
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp,
			&tex->line_len, &tex->endian);
	if (!tex->addr)
	{
		mlx_destroy_image(club->mlx, tex->img);
		tex->img = NULL;
		return (1);
	}
	return (0);
}

static void	try_load_tex(t_club *club, t_tex *tex, int *fail, int mandatory)
{
	int	err;

	if (!tex || !tex->path)
	{
		if (mandatory)
			*fail = 1;
		return ;
	}
	err = load_texture(club, tex);
	if (err)
	{
		tex->img = NULL;
		tex->addr = NULL;
		if (mandatory)
			*fail = 1;
	}
}

int	load_all_textures(t_club *club)
{
	int	fail;

	fail = 0;
	try_load_tex(club, &club->tex[TEX_NO], &fail, 1);
	try_load_tex(club, &club->tex[TEX_SO], &fail, 1);
	try_load_tex(club, &club->tex[TEX_WE], &fail, 1);
	try_load_tex(club, &club->tex[TEX_EA], &fail, 1);
	try_load_tex(club, &club->tex[TEX_CEIL], &fail, 0);
	if (have_door(club))
		try_load_tex(club, &club->tex[TEX_FLOOR], &fail, 0);
	return (fail);
}

static void	destroy_one_tex(t_club *c, t_tex *t, int free_path)
{
	if (t->img)
	{
		mlx_destroy_image(c->mlx, t->img);
		t->img = NULL;
		t->addr = NULL;
	}
	if (free_path && t->path)
	{
		free(t->path);
		t->path = NULL;
	}
}

void	destroy_textures(t_club *club)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		destroy_one_tex(club, &club->tex[i], 0);
		i++;
	}
	destroy_one_tex(club, &club->door_tex, 1);
	destroy_one_tex(club, &club->door_open_tex, 1);
}
