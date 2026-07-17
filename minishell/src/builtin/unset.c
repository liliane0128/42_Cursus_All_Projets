/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:40:05 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 16:40:08 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	find_env_index(char **envp, const char *var)
{
	int		i;
	size_t	len;

	len = ft_strlen(var);
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strcmp(envp[i], var) == 0)
			return (i);
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	error_msg(const char *cmd, const char *arg, const char *msg, int status)
{
	if (cmd)
		ft_putstr_fd((char *)cmd, STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
	}
	if (msg)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd((char *)msg, STDERR_FILENO);
	}
	else
		ft_putendl_fd("", STDERR_FILENO);
	return (status);
}

static void	unset_var(t_data *data, const char *var)
{
	int	i;

	if (ft_strchr(var, '=') || !is_valid_identifier(var))
	{
		data->last_status = error_msg("unset", var, \
				"not a valid argument", 1);
		return ;
	}
	i = find_env_index(data->envp, var);
	if (i != -1)
	{
		free(data->envp[i]);
		while (data->envp[i + 1])
		{
			data->envp[i] = data->envp[i + 1];
			i++;
		}
		data->envp[i] = NULL;
	}
}

int	unset_builtin(t_data *data, char **args)
{
	int	i;

	if (!data || !data->envp)
		return (EXIT_FAILURE);
	data->last_status = 0;
	i = 1;
	while (args[i])
	{
		unset_var(data, args[i]);
		i++;
	}
	return (data->last_status);
}
