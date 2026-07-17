/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:05 by hguo              #+#    #+#             */
/*   Updated: 2026/01/26 15:51:40 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	count_lines(const char *file)
{
	int		fd;
	int		count;
	char	*line;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (-1);
	count = 0;
	line = get_next_line(fd);
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

static char	**load_lines(const char *file, int size)
{
	char	**res;
	char	*line;
	int		fd;
	int		i;

	res = malloc(sizeof(char *) * (size + 1));
	if (!res)
		return (NULL);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (free(res), NULL);
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		res[i] = line;
		i++;
		line = get_next_line(fd);
	}
	res[i] = NULL;
	close(fd);
	return (res);
}

char	**read_file(const char *file)
{
	int		size;
	char	**res;

	size = count_lines(file);
	if (size < 0)
		return (NULL);
	res = load_lines(file, size);
	return (res);
}

static int	check_elements_complete(t_club *club)
{
	if (!club->tex[TEX_NO].path || !club->tex[TEX_SO].path \
		|| !club->tex[TEX_WE].path || !club->tex[TEX_EA].path)
		return (err_msg("Error: Missing texture(s)"), -1);
	if (club->floor_color == -1 || club->ceiling_color == -1)
		return (err_msg("Error: Missing color(s)"), -1);
	return (0);
}

int	parsing(t_club *club, char **file)
{
	if (!file || !*file)
		return (err_msg("Error: No file content provided"), -1);
	club->floor_color = -1;
	club->ceiling_color = -1;
	if (check_elements(club, file) == -1)
		return (-1);
	if (check_elements_complete(club) == -1)
		return (-1);
	if (get_map(club, file) == -1)
		return (err_msg("Error: Map missing"), -1);
	if (!is_map_at_the_end(club, file))
		return (err_msg("Error: Map is not at the end of the file"), -1);
	if (!check_map_valid(club))
		return (-1);
	if (!prepare_map(club))
		return (-1);
	return (0);
}
