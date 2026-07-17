/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 13:43:36 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/17 15:49:26 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	save_stdio(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin < 0)
		return (perror("dup"), false);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout < 0)
	{	
		close(*saved_stdin);
		return (perror("dup"), false);
	}
	return (true);
}

void	restore_stdio(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != STDIN_FILENO)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != STDOUT_FILENO)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

bool	cmd_is_dir(char *cmd)
{
	struct stat	st;

	if (!cmd)
		return (false);
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		return (true);
	return (false);
}

bool	is_builtin(const char *cmd)
{
	return (cmd && (
			ft_strcmp(cmd, "cd") == 0
			|| ft_strcmp(cmd, "echo") == 0
			|| ft_strcmp(cmd, "pwd") == 0
			|| ft_strcmp(cmd, "export") == 0
			|| ft_strcmp(cmd, "unset") == 0
			|| ft_strcmp(cmd, "env") == 0
			|| ft_strcmp(cmd, "exit") == 0));
}

void	close_redirs(t_redir *redirs)
{
	t_redir	*cur;

	cur = redirs;
	while (cur)
	{
		if (cur->type == REDIR_HEREDOC && cur->heredoc_fd >= 0)
		{
			close(cur->heredoc_fd);
			cur->heredoc_fd = -1;
		}
		cur = cur->next;
	}
}
