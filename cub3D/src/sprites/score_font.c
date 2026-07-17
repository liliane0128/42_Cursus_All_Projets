/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   score_font.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:49 by hguo              #+#    #+#             */
/*   Updated: 2026/01/23 11:41:31 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/**
 * Returns the 5-bit wide bitmap row for a given character.
 * Each character is a 5x7 sprite defined by hexadecimal rows.
 * Used for rendering a simple "SCORE" UI without external assets.
 * * @param c The character to retrieve (S, C, O, R, or E).
 * @param row The specific row index (0-6) of the 7-pixel high font.
 */
static unsigned char	get_font_data(char c, int row)
{
	static const unsigned char s[7] \
		= {0x0E, 0x11, 0x10, 0x0E, 0x01, 0x11, 0x0E};
	static const unsigned char c_f[7] \
		= {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E};
	static const unsigned char o[7] \
		= {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E};
	static const unsigned char r[7] \
		= {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11};
	static const unsigned char e[7] \
		= {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F};
	if (c == 'S')
		return (s[row]);
	if (c == 'C')
		return (c_f[row]);
	if (c == 'O')
		return (o[row]);
	if (c == 'R')
		return (r[row]);
	if (c == 'E')
		return (e[row]);
	return (0);
}

static void	draw_scaled_pixel(t_img *img, int pos[2], int color, int scale)
{
	int	px;
	int	py;

	py = 0;
	while (py < scale)
	{
		px = 0;
		while (px < scale)
		{
			put_pixel(img, pos[0] + px, pos[1] + py, color);
			px++;
		}
		py++;
	}
}

static void	draw_glyph5x7(t_img *img, int pos[2], char c, int params[2])
{
	int	r;
	int	col;
	int	draw_pos[2];

	r = 0;
	while (r < 7)
	{
		col = 0;
		while (col < 5)
		{
			if (get_font_data(c, r) & (1 << (4 - col)))
			{
				draw_pos[0] = pos[0] + col * params[1];
				draw_pos[1] = pos[1] + r * params[1];
				draw_scaled_pixel(img, draw_pos, params[0], params[1]);
			}
			col++;
		}
		r++;
	}
}

void	ui_draw_text_score(t_img *img, int pos[2], int color, int scale)
{
	int		step;
	char	*str;
	int		i;
	int		curr_p[2];

	step = (5 * scale) + (2 * scale);
	str = "SCORE";
	i = 0;
	curr_p[1] = pos[1];
	while (str[i])
	{
		curr_p[0] = pos[0] + (i * step);
		draw_glyph5x7(img, curr_p, str[i], (int [2]){color, scale});
		i++;
	}
}
