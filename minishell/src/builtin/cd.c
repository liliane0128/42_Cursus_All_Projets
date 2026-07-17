/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:52:32 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 10:43:22 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_new_var(const char *key, const char *value)
{
	char	*str;
	size_t	len;

	if (!key || !value)
		return (NULL);
	len = ft_strlen(key) + ft_strlen(value) + 2;
	str = malloc(len);
	if (!str)
		return (NULL);
	ft_strcpy(str, key);
	ft_strcat(str, "=");
	ft_strcat(str, value);
	return (str);
}

static int	set_env(t_data *data, const char *key, const char *value)
{
	int		i;
	char	*new_var;

	new_var = get_new_var(key, value);
	if (!new_var)
		return (-1);
	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], key, ft_strlen(key)) == 0 \
				&& data->envp[i][ft_strlen(key)] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = new_var;
			return (0);
		}
		i++;
	}
	data->envp = ft_realloc_env(data->envp, new_var);
	if (!data->envp)
		return (free(new_var), -1);
	return (0);
}

char	*get_env_value(char **envp, const char *key)
{
	int		i;
	size_t	len;

	if (!key || !envp)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (NULL);
}

static char	*get_cd_path(t_data *data, char **args)
{
	if (!args[1] || ft_strcmp(args[1], "--") == 0 \
			|| ft_strcmp(args[1], "~") == 0)
		return (get_env_value(data->envp, "HOME"));
	if (ft_strcmp(args[1], "-") == 0)
		return (get_env_value(data->envp, "OLDPWD"));
	return (ft_strdup(args[1]));
}

int	cd_builtin(t_data *data, char **args)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;

	if (args[1] && args[2])
		return (errmsg_cmd("cd", NULL, "too many arguments", 1));
	oldpwd = getcwd(NULL, 0);
	path = get_cd_path(data, args);
	if (!path || chdir(path) == -1)
	{
		free(oldpwd);
		free(path);
		errmsg_cmd("cd", args[1], "No such file or directory", 127);
		return (EXIT_FAILURE);
	}
	newpwd = getcwd(NULL, 0);
	if (set_env(data, "OLDPWD", oldpwd) == -1 \
			|| set_env(data, "PWD", newpwd) == -1)
		return (free(oldpwd), free(newpwd), free(path), EXIT_FAILURE);
	if (args[1] && ft_strcmp(args[1], "-") == 0)
		printf("%s\n", newpwd);
	free(oldpwd);
	free(newpwd);
	free(path);
	return (EXIT_SUCCESS);
}
