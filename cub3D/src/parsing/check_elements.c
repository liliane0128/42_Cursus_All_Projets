/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hguo <hguo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:29:36 by hguo              #+#    #+#             */
/*   Updated: 2026/01/19 16:29:37 by hguo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static bool	is_texture_key(char *line)
{
	if (!line)
		return (false);
	while (ft_is_whitespace(*line))
		line++;
	if (*line == 'N' && *(line + 1) == 'O')
		return (true);
	if (*line == 'S' && *(line + 1) == 'O')
		return (true);
	if (*line == 'W' && *(line + 1) == 'E')
		return (true);
	if (*line == 'E' && *(line + 1) == 'A')
		return (true);
	return (false);
}

static bool	is_color_key(char *line)
{
	if (!line)
		return (false);
	while (ft_is_whitespace(*line))
		line++;
	if (*line == 'F' || *line == 'C')
		return (true);
	return (false);
}

int	check_elements(t_club *club, char **lines)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		if (is_empty_line(lines[i]))
			;
		else if (is_texture_key(lines[i]))
		{
			if (parse_texture(lines[i], club) == -1)
				return (-1);
		}
		else if (is_color_key(lines[i]))
		{
			if (parse_color(lines[i], club) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
