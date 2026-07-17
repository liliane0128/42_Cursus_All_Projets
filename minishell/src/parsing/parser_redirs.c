/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:41:19 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 16:41:19 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir_type	get_redir_type(t_token *tok)
{
	if (!tok)
		return (REDIR_NONE);
	if (tok->type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	else if (tok->type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	else if (tok->type == TOKEN_APPEND)
		return (REDIR_APPEND);
	else if (tok->type == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_NONE);
}

static t_redir	*create_redir(t_redir_type type, char *target)
{
	t_redir	*redir;

	if (!target)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = ft_strdup(target);
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	redir->heredoc_fd = -1;
	redir->heredoc_quote = NO_QUOTE;
	redir->next = NULL;
	return (redir);
}

/*
** append_redir() adds a redirection to the end of the cmd's redir list.
*/
static void	append_redir(t_cmd *cmd, t_redir *redir)
{
	t_redir	*last;

	if (!cmd->redirs)
	{
		cmd->redirs = redir;
		return ;
	}
	last = cmd->redirs;
	while (last->next)
		last = last->next;
	last->next = redir;
}

/*
** handle_token_redir() creates a redirection from a token and
** appends it to the command. Returns true on success, false on error.
*/
static bool	handle_token_redir(t_cmd *cmd, t_token *token)
{
	t_redir_type	type;
	t_redir			*redir;

	if (!token || !token->next || !token->next->value)
		return (false);
	type = get_redir_type(token);
	if (type == REDIR_NONE)
		return (true);
	redir = create_redir(type, token->next->value);
	if (!redir)
		return (false);
	if (type == REDIR_HEREDOC)
	{
		redir->heredoc_fd = -1;
		redir->heredoc_quote = get_heredoc_quote(token->next->value);
	}
	append_redir(cmd, redir);
	return (true);
}

bool	parse_redirections(t_cmd *cmd, t_token **tokens, t_data *data)
{
	t_token	*cur;

	(void)data;
	if (!cmd || !tokens || !*tokens)
		return (false);
	cur = *tokens;
	while (cur && !is_cmd_end(cur))
	{
		if (get_redir_type(cur) != REDIR_NONE)
		{
			if (!cur->next || !handle_token_redir(cmd, cur))
			{
				free_redirs(cmd->redirs);
				cmd->redirs = NULL;
				return (false);
			}
			cur = cur->next->next;
		}
		else
		cur = cur->next;
	}
	return (true);
}
