/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:02:52 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 11:33:32 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_last_code = 0;

static void	init_data(t_data *data, char **envp)
{
	data->envp = ft_envp_dup(envp);
	if (!data->envp)
		return ;
	data->cmd = NULL;
	data->ast = NULL;
	data->saved_stdin = -1;
	data->saved_stdout = -1;
	data->last_status = 0;
}

static void	reset_ast(t_data *data)
{
	if (data->ast)
	{
		free_ast(data->ast);
		data->ast = NULL;
	}
}

static char	*read_input(t_data *data)
{
	char	*line;

	if (g_last_code)
	{
		data->last_status = g_last_code;
		g_last_code = 0;
	}
	line = readline("minishell$ ");
	if (!line)
	{
		printf("exit\n");
		rl_clear_history();
		return (NULL);
	}
	if (*line)
		add_history(line);
	return (line);
}

void	minishell_loop(t_data *data)
{
	char	*line;

	while (1)
	{
		line = read_input(data);
		if (!line)
			return ;
		if (g_last_code == 130)
		{
			reset_ast(data);
			free(line);
			continue ;
		}
		reset_ast(data);
		data->ast = parse(line, data);
		if (!data->ast)
		{
			free(line);
			continue ;
		}
		data->last_status = exec_ast(data->ast, data);
		reset_ast(data);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	init_data(&data, envp);
	setup_parent_signals();
	minishell_loop(&data);
	cleanup(&data);
	return (data.last_status);
}
