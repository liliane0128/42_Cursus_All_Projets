/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   img_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:26:43 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:26:44 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	init_image(t_club *club)
{
	club->img.img = mlx_new_image(club->mlx, WIDTH, HEIGHT);
	if (!club->img.img)
		return (1);
	club->img.addr = mlx_get_data_addr(club->img.img, &club->img.bpp,
			&club->img.line_len, &club->img.endian);
	if (!club->img.addr)
		return (1);
	return (0);
}

void	destroy_image(t_club *club)
{
	if (club->img.img)
	{
		mlx_destroy_image(club->mlx, club->img.img);
		club->img.img = NULL;
		club->img.addr = NULL;
	}
}

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)dst = color;
}

// 每个字节有8位
// 转换为int*类型方便一次性写入像素颜色