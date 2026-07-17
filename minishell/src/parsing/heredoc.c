/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:41:07 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 16:41:07 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_line_to_pipe(int fd, char *line,
		t_quote_status q_type, t_data *data)
{
	char	*expanded;

	if (q_type != SINGLE_QUOTE)
	{
		expanded = expand_heredoc(data, line);
		write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static bool	read_heredoc_to_pipe(int write_fd, char *delim,
				t_quote_status q_type, t_data *data)
{
	char	*line;

	while (g_last_code != 130)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			if (!line)
				errmsg_cmd("warning", delim,
					"heredoc delimited by EOF (wanted `')", 0);
			free(line);
			break ;
		}
		write_line_to_pipe(write_fd, line, q_type, data);
		free(line);
	}
	return (g_last_code != 130);
}

int	process_heredoc(char *token, t_quote_status q_type, t_data *data)
{
	char	*delim;
	int		pipefd[2];
	bool	success;

	delim = trim_quotes(token);
	if (!delim)
		return (-1);
	if (pipe(pipefd) < 0)
		return (perror("pipe"), free(delim), -1);
	g_last_code = 0;
	rl_event_hook = ctrlc_event_hook;
	setup_heredoc_signals();
	success = read_heredoc_to_pipe(pipefd[1], delim, q_type, data);
	setup_parent_signals();
	rl_event_hook = NULL;
	close(pipefd[1]);
	free(delim);
	if (!success)
	{
		close(pipefd[0]);
		g_last_code = 130;
		return (-1);
	}
	return (pipefd[0]);
}

bool	preprocess_heredocs(t_ast *node, t_data *data)
{
	t_redir	*re;

	if (!node)
		return (true);
	if (!preprocess_heredocs(node->left, data))
		return (false);
	if (!preprocess_heredocs(node->right, data))
		return (false);
	if (node->type == NODE_CMD && node->cmd)
	{
		re = node->cmd->redirs;
		while (re)
		{
			if (re->type == REDIR_HEREDOC && re->heredoc_fd < 0)
			{
				re->heredoc_fd = process_heredoc(re->target,
						re->heredoc_quote, data);
				if (re->heredoc_fd < 0)
					return (false);
			}
			re = re->next;
		}
	}
	return (true);
}
