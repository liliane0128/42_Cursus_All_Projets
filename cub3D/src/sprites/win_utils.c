/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   win_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:29:10 by hguo              #+#    #+#             */
/*   Updated: 2026/01/23 15:52:41 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	load_win_banner(t_club *c)
{
	if (c->win_loaded)
		return (0);
	if (!is_valid_xpm_file("xpms/you_win.xpm"))
		return (err_msg("Win Xpm file not found or invalid"), 1);
	c->win_img.img = mlx_xpm_file_to_image(c->mlx, "xpms/you_win.xpm",
			&c->win_img.width, &c->win_img.height);
	if (!c->win_img.img)
		return (1);
	c->win_img.addr = mlx_get_data_addr(c->win_img.img, &c->win_img.bpp,
			&c->win_img.line_len, &c->win_img.endian);
	if (!c->win_img.addr)
	{
		mlx_destroy_image(c->mlx, c->win_img.img);
		c->win_img.img = NULL;
		return (1);
	}
	c->win_loaded = 1;
	return (0);
}
