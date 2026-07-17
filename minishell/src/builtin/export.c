/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:39:48 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 14:46:16 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (false);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static int	set_env_var(t_data *data, const char *arg)
{
	int		i;
	size_t	len;
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
		len = eq - arg;
	else
		len = ft_strlen(arg);
	i = 0;
	while (data->envp[i])
	{
		if (!ft_strncmp(data->envp[i], arg, len) && data->envp[i][len] == '=')
		{
			free(data->envp[i]);
			data->envp[i] = ft_strdup(arg);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	data->envp = ft_realloc_env(data->envp, arg);
	return (EXIT_SUCCESS);
}

static void	print_export(char **envp)
{
	int		i;
	char	**sorted;

	sorted = ft_sort_env(envp);
	i = 0;
	while (sorted[i])
	{
		printf("export %s\n", sorted[i]);
		i++;
	}
	ft_free_array(sorted);
}

int	export_builtin(t_data *data, char **args)
{
	int	i;
	int	status;

	i = 1;
	if (!args[i])
	{
		print_export(data->envp);
		return (EXIT_SUCCESS);
	}
	status = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
			status = errmsg_cmd_w_quote("export", args[i], \
				"not a valid identifier", 1);
		else
			set_env_var(data, args[i]);
		i++;
	}
	data->last_status = status;
	return (status);
}
