/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:29:48 by hguo              #+#    #+#             */
/*   Updated: 2026/01/26 15:51:20 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static bool	is_all_number(char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (false);
	while (ft_is_whitespace(*s))
		s++;
	if (*s == '\0')
		return (false);
	i = 0;
	while (s[i] && !ft_is_whitespace(s[i]))
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}

static bool	validate_color_values(char **nums, int *r, int *g, int *b)
{
	if (!nums || !nums[0] || !nums[1] || !nums[2] || nums[3])
		return (err_msg("Error: Color should have exactly 3 components"), \
		false);
	if (nums[0][0] == '\0' || nums[1][0] == '\0' || nums[2][0] == '\0')
		return (err_msg("Error: Color should have exactly 3 components"), \
		false);
	if (nums[0][0] == '\n' || nums[1][0] == '\n' || nums[2][0] == '\n')
		return (err_msg("Error: Color should have exactly 3 components"), \
		false);
	if (!is_all_number(nums[0]) || !is_all_number(nums[1]) \
			|| !is_all_number(nums[2]))
		return (err_msg("Error: Color values must be positif numbers"), false);
	*r = ft_atoi(nums[0]);
	*g = ft_atoi(nums[1]);
	*b = ft_atoi(nums[2]);
	if (*r < 0 || *r > 255 || *g < 0 || *g > 255 || *b < 0 || *b > 255)
		return (err_msg("Error: Color values out of range: 0 ~ 255"), false);
	return (true);
}

int	parse_color(char *line, t_club *club)
{
	char	**nums;
	char	type;
	int		rgb[3];

	while (*line == ' ')
		line++;
	if (*line != 'F' && *line != 'C')
		return (err_msg("Error: unknown color type"), -1);
	type = *line;
	if (type == 'F' && club->floor_color != -1)
		return (err_msg("Error: floor color defined multiple times"), -1);
	if (type == 'C' && club->ceiling_color != -1)
		return (err_msg("Error: ceiling color defined multiple times"), -1);
	line++;
	while (*line == ' ')
		line++;
	nums = ft_split(line, ',');
	if (!validate_color_values(nums, &rgb[0], &rgb[1], &rgb[2]))
		return (free_array(nums), -1);
	if (type == 'F')
		club->floor_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	else
		club->ceiling_color = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	free_array(nums);
	return (0);
}
