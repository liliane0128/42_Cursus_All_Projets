/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   score_bar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:43 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:30:45 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	score_step(int scale)
{
	return ((5 * scale) + (2 * scale));
}

int	ui_score_label_width(int scale)
{
	return (score_step(scale) * 5);
}

static void	bar_clamp(t_bar *b)
{
	if (b->total <= 0)
		b->total = 1;
	if (b->cur < 0)
		b->cur = 0;
	if (b->cur > b->total)
		b->cur = b->total;
}

void	ui_draw_progress_bar(t_img *img, t_bar *b)
{
	t_rect	inner;
	int		fill_w;

	bar_clamp(b);
	ui_draw_rect(img, b->r, b->bg);
	ui_draw_border(img, b->r, b->border, 2);
	inner.x = b->r.x + 3;
	inner.y = b->r.y + 3;
	inner.w = b->r.w - 6;
	inner.h = b->r.h - 6;
	ui_draw_border(img, inner, b->inner, 1);
	inner.x += 2;
	inner.y += 2;
	inner.w -= 4;
	inner.h -= 4;
	ui_draw_rect(img, inner, b->bg);
	fill_w = (inner.w * b->cur) / b->total;
	if (fill_w > 0)
	{
		inner.w = fill_w;
		ui_draw_rect(img, inner, b->fill);
	}
}
