/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:06:16 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 16:02:20 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_builtin(t_data *data, char **args)
{
	int	i;

	if (!data || !data->envp)
		return (EXIT_FAILURE);
	if (args && args[1])
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd("': No such file or directory\n", 2);
		return (127);
	}
	i = 0;
	while (data->envp[i])
	{
		if (ft_strchr(data->envp[i], '='))
			ft_putendl_fd(data->envp[i], STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
