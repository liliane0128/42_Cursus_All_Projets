/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_club.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:14:57 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/23 17:09:16 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	destroy_sprite_textures(t_club *club)
{
	int	i;

	if (!club || !club->mlx)
		return ;
	i = 0;
	while (i < SPRITE_TYPES)
	{
		if (club->sprite_textures[i].img)
		{
			mlx_destroy_image(club->mlx, club->sprite_textures[i].img);
			club->sprite_textures[i].img = NULL;
		}
	club->sprite_textures[i].addr = NULL;
	club->sprite_textures[i].bpp = 0;
	club->sprite_textures[i].line_len = 0;
	club->sprite_textures[i].endian = 0;
	club->sprite_textures[i].width = 0;
	club->sprite_textures[i].height = 0;
	i++;
	}
}

void	destroy_club(t_club *club)
{
	if (!club)
		return ;
	if (club->img.img)
		mlx_destroy_image(club->mlx, club->img.img);
	if (club->win_img.img)
		mlx_destroy_image(club->mlx, club->win_img.img);
	destroy_sprites(club);
	destroy_doors(club);
	destroy_textures_club(club);
	destroy_sprite_textures(club);
	destroy_map(club);
	if (club->win)
		mlx_destroy_window(club->mlx, club->win);
	if (club->mlx)
	{
		mlx_destroy_display(club->mlx);
		free(club->mlx);
		club->mlx = NULL;
	}
}
