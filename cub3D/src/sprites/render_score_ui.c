/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_score_ui.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:35 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:30:36 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	score_count_found(t_club *club)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (i < club->sprite_count)
	{
		if (club->sprites[i].found)
			found++;
		i++;
	}
	return (found);
}

static t_rect	score_panel_rect(void)
{
	t_rect	r;

	r.x = WIDTH - SCORE_W - SCORE_X_MARGIN;
	r.y = SCORE_Y_MARGIN;
	r.w = SCORE_W;
	r.h = SCORE_H;
	return (r);
}

void	render_score_ui(t_club *club)
{
	t_rect	p;
	t_bar	bar;
	int		scale;
	int		found;

	scale = 2;
	p = score_panel_rect();
	found = score_count_found(club);
	ui_draw_rect(&club->img, p, SCORE_BG);
	ui_draw_border(&club->img, p, SCORE_BORDER, 2);
	ui_draw_text_score(&club->img, (int [2]){p.x + SCORE_PAD, \
		p.y + ((p.h - (7 * scale)) / 2)}, SCORE_TEXT, scale);
	bar.r.x = p.x + SCORE_PAD + ui_score_label_width(scale) + 12;
	bar.r.y = p.y + 8;
	bar.r.w = p.x + p.w - SCORE_PAD - bar.r.x;
	bar.r.h = p.h - 16;
	bar.cur = found;
	bar.total = club->sprite_count;
	bar.bg = SCORE_BG;
	bar.border = SCORE_BORDER;
	bar.inner = SCORE_INNER;
	bar.fill = SCORE_FILL;
	ui_draw_progress_bar(&club->img, &bar);
}
