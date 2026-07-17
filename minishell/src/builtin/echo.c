/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:25:03 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/17 14:05:58 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
*   is_n_option:
*	Checks if the given argument is a valid -n option
*	Supports -n, -nn, -nnn, etc.
*/
static bool	is_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] == '\0' )
		return (false);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	echo_builtin(t_data *data, char **args)
{
	int	i;
	int	flag_n;

	(void)data;
	i = 1;
	flag_n = 0;
	while (args[i] && is_n_option(args[i]))
	{
		flag_n = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!flag_n)
		printf("\n");
	return (EXIT_SUCCESS);
}
