/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 17:37:09 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/17 15:27:59 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_join_path_cmd(char *dir, char *cmd)
{
	char	*path;
	int		len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = malloc(len);
	if (!path)
		return (NULL);
	ft_strcpy(path, dir);
	ft_strcat(path, "/");
	ft_strcat(path, cmd);
	return (path);
}

static char	**get_path_dirs(char **envp)
{
	char	*path_value;
	char	**dirs;

	path_value = NULL;
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
		{
			path_value = ft_strdup(*envp + 5);
			break ;
		}
		envp++;
	}
	if (!path_value)
		return (NULL);
	dirs = ft_split(path_value, ':');
	free(path_value);
	return (dirs);
}

static char	*search_in_dirs(char *cmd, char **dirs)
{
	char	*full;
	int		i;

	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full = ft_join_path_cmd(dirs[i], cmd);
		if (!full)
			return (ft_free_array(dirs), NULL);
		if (access(full, F_OK) == 0)
		{
			if (cmd_is_dir(full))
			{
				free(full);
				i++;
				continue ;
			}
			return (ft_free_array(dirs), full);
		}
		free(full);
		i++;
	}
	return (ft_free_array(dirs), NULL);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	**dirs;
	char	*dup;

	if (ft_strchr(cmd, '/'))
	{
		dup = ft_strdup(cmd);
		if (!dup)
			return (NULL);
		if (cmd_is_dir(dup))
		{
			ft_putstr_fd(dup, 2);
			ft_putendl_fd(": Is a directory", 2);
			free(dup);
			return (NULL);
		}
		return (dup);
	}
	dirs = get_path_dirs(envp);
	return (search_in_dirs(cmd, dirs));
}
