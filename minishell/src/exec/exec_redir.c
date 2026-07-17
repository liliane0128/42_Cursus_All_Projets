/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 18:41:02 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 12:37:38 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	redir_in(t_redir *re)
{
	int	fd;

	fd = open(re->target, O_RDONLY);
	if (fd < 0)
		return (perror(re->target), false);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (perror("dup2"), false);
	}
	close(fd);
	return (true);
}

static bool	redir_out(t_redir *re)
{
	int	fd;

	fd = open(re->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(re->target), false);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror("dup2"), false);
	}
	close(fd);
	return (true);
}

static bool	redir_append(t_redir *re)
{
	int	fd;

	fd = open(re->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(re->target), false);
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror("dup2"), false);
	}
	close(fd);
	return (true);
}

static bool	redir_heredoc(t_redir *re)
{
	if (!re)
		return (false);
	if (re->heredoc_fd <= 0)
	{
		g_last_code = 130;
		return (true);
	}
	if (dup2(re->heredoc_fd, STDIN_FILENO) < 0)
		return (false);
	if (close(re->heredoc_fd) < 0)
		return (false);
	re->heredoc_fd = -1;
	return (true);
}

bool	handle_redirs(t_cmd *cmd)
{
	t_redir	*re;
	char	*tmp;

	if (!cmd)
		return (false);
	re = cmd->redirs;
	while (re)
	{
		if (re->target)
		{
			tmp = remove_quotes(re->target);
			free(re->target);
			re->target = tmp;
		}
		if (re->type == REDIR_IN && !redir_in(re))
			return (false);
		else if (re->type == REDIR_OUT && !redir_out(re))
			return (false);
		else if (re->type == REDIR_APPEND && !redir_append(re))
			return (false);
		else if (re->type == REDIR_HEREDOC && !redir_heredoc(re))
			return (false);
		re = re->next;
	}
	return (true);
}
