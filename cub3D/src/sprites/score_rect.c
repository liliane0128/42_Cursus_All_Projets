/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   score_rect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:52 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:30:53 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ui_draw_rect(t_img *img, t_rect r, int color)
{
	int	i;
	int	j;

	j = 0;
	while (j < r.h)
	{
		i = 0;
		while (i < r.w)
		{
			put_pixel(img, r.x + i, r.y + j, color);
			i++;
		}
		j++;
	}
}

static void	draw_hline(t_img *img, int pos[2], int w, int color)
{
	int	i;

	i = 0;
	while (i < w)
	{
		put_pixel(img, pos[0] + i, pos[1], color);
		i++;
	}
}

static void	draw_vline(t_img *img, int pos[2], int h, int color)
{
	int	i;

	i = 0;
	while (i < h)
	{
		put_pixel(img, pos[0], pos[1] + i, color);
		i++;
	}
}

void	ui_draw_border(t_img *img, t_rect r, int color, int t)
{
	int	k;

	k = 0;
	while (k < t)
	{
		draw_hline(img, (int [2]){r.x, r.y + k}, r.w, color);
		draw_hline(img, (int [2]){r.x, r.y + r.h - 1 - k}, r.w, color);
		draw_vline(img, (int [2]){r.x + k, r.y}, r.h, color);
		draw_vline(img, (int [2]){r.x + r.w - 1 - k, r.y}, r.h, color);
		k++;
	}
}
