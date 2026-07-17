/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 11:46:31 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/19 11:57:43 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	errmsg_w_quote(char *cmd, char *msg, char *arg, int status)
{
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putstr_fd(msg, 2);
	if (arg)
	{
		ft_putstr_fd("'", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
	}
	ft_putchar_fd('\n', 2);
	return (status);
}

bool	check_empty_pipe(t_token *tokens, t_data *data)
{
	t_token	*cur;
	t_token	*prev;

	cur = tokens;
	prev = NULL;
	while (cur)
	{
		if (cur->type == TOKEN_PIPE)
		{
			if (!prev || !prev->value || prev->type == TOKEN_PIPE)
				return (data->last_status = \
					errmsg_cmd("minishell", "|", ERR_MSG, 2), false);
			if (!cur->next)
				return (true);
			if (!cur->next->value || cur->next->type == TOKEN_PIPE)
			{
				data->last_status = errmsg_cmd("minishell", "|", ERR_MSG, 2);
				return (false);
			}
		}
		prev = cur;
		cur = cur->next;
	}
	return (true);
}

static bool	is_invalid_redir_follower(t_token *next)
{
	if (next->type == TOKEN_OR || next->type == TOKEN_AND
		|| next->type == TOKEN_PIPE || next->type == TOKEN_REDIR_IN
		|| next->type == TOKEN_REDIR_OUT || next->type == TOKEN_APPEND
		|| next->type == TOKEN_HEREDOC)
		return (true);
	return (false);
}

static bool	handle_pipe(t_token *cur, t_token *next, \
	t_token *start, t_data *data)
{
	if (cur == start)
		return (data->last_status = errmsg_w_quote(NULL, ERR_MSG,
				cur->value, 2), false);
	if (!next)
		return (data->last_status = errmsg_w_quote(NULL, ERR_MSG,
				"newline", 2), false);
	if (next->type == TOKEN_PIPE || next->type == TOKEN_OR
		|| next->type == TOKEN_AND)
		return (data->last_status = errmsg_w_quote(NULL, ERR_MSG,
				next->value, 2), false);
	return (true);
}

bool	check_redir_tokens(t_token *tokens, t_data *data)
{
	t_token	*cur;
	t_token	*next;

	cur = tokens;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOKEN_REDIR_IN || cur->type == TOKEN_REDIR_OUT
			|| cur->type == TOKEN_APPEND || cur->type == TOKEN_HEREDOC)
		{
			if (!next)
				return (data->last_status = errmsg_w_quote(NULL,
						ERR_MSG, "newline", 2), false);
			if (is_invalid_redir_follower(next))
				return (data->last_status = errmsg_w_quote(NULL,
						ERR_MSG, next->value, 2), false);
		}
		else if (cur->type == TOKEN_PIPE
			&& !handle_pipe(cur, next, tokens, data))
			return (false);
		cur = cur->next;
	}
	return (true);
}
