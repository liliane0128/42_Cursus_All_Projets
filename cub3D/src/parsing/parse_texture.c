/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:29:57 by hguo              #+#    #+#             */
/*   Updated: 2026/01/23 12:16:25 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

bool	is_valid_xpm_file(char *path)
{
	int	len;
	int	fd;

	if (!path)
		return (false);
	len = ft_strlen(path);
	if (len < 4 || ft_strcmp(path + len - 4, ".xpm") != 0)
		return (false);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

static char	*extract_path(char *line)
{
	char	*path;
	int		start;
	int		len;

	start = 0;
	while (ft_is_whitespace(line[start]))
		start++;
	len = 0;
	while (line[start + len] != '\0' && !ft_is_whitespace(line[start + len]))
		len++;
	path = malloc(len + 1);
	if (!path)
		return (NULL);
	len = 0;
	while (line[start + len] != '\0' && !ft_is_whitespace(line[start + len]))
	{
		path[len] = line[start + len];
		len++;
	}
	path[len] = '\0';
	return (path);
}

static t_tex	*get_texture_ptr(t_club *club, char *line)
{
	while (ft_is_whitespace(*line))
		line++;
	if (ft_strncmp(line, "NO", 2) == 0)
		return (&club->tex[0]);
	else if (ft_strncmp(line, "SO", 2) == 0)
		return (&club->tex[1]);
	else if (ft_strncmp(line, "WE", 2) == 0)
		return (&club->tex[2]);
	else if (ft_strncmp(line, "EA", 2) == 0)
		return (&club->tex[3]);
	return (NULL);
}

int	parse_texture(char *line, t_club *club)
{
	t_tex	*tex_ptr;
	char	*path;

	if (*line == '\0')
		return (err_msg("Error: empty texture line"), -1);
	tex_ptr = get_texture_ptr(club, line);
	if (!tex_ptr)
		return (err_msg("Error: unknown texture type"), -1);
	while (ft_is_whitespace(*line))
		line++;
	line += 2;
	while (ft_is_whitespace(*line))
		line++;
	path = extract_path(line);
	if (!path)
		return (err_msg("Error: texture path missing"), -1);
	if (!is_valid_xpm_file(path))
		return (free(path), err_msg("Error: invalid xmp file"), -1);
	if (tex_ptr->path)
		return (free(path), err_msg("Error: texture already defined"), -1);
	tex_ptr->path = path;
	return (0);
}
