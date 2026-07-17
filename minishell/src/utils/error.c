/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 13:34:01 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/17 15:44:58 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	errmsg_cmd(char *cmd, char *arg, char *msg, int status)
{
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	return (status);
}

int	errmsg_cmd_w_quote(char *cmd, char *arg, char *msg, int status)
{
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd("'", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
		ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	return (status);
}

void	cleanup_and_exit(int status, t_data *data, int *fds, int fds_count)
{
	int	i;

	i = 0;
	while (i < fds_count)
	{
		if (fds[i] >= 0)
			close(fds[i]);
		i++;
	}
	if (data)
	{
		if (data->ast)
		{
			free_ast(data->ast);
			data->ast = NULL;
		}
		if (data->envp)
			ft_free_array(data->envp);
		if (data->saved_stdin != -1)
			close(data->saved_stdin);
		if (data->saved_stdout != -1)
			close(data->saved_stdout);
	}
	exit(status);
}

void	execve_error(t_data *data, char *path)
{
	if (errno == EISDIR)
	{
		perror(data->cmd->av[0]);
		free(path);
		cleanup_and_exit(126, data, NULL, 0);
	}
	if (errno == EACCES)
	{
		perror(data->cmd->av[0]);
		free(path);
		cleanup_and_exit(126, data, NULL, 0);
	}
	if (errno == ENOENT)
	{
		perror(data->cmd->av[0]);
		free(path);
		cleanup_and_exit(127, data, NULL, 0);
	}
	else
	{
		perror(data->cmd->av[0]);
		free(path);
		cleanup_and_exit(1, data, NULL, 0);
	}
}
