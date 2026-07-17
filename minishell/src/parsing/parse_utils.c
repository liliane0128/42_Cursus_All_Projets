/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 21:06:43 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 11:55:19 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->type == REDIR_HEREDOC && tmp->heredoc_fd >= 0)
			close(tmp->heredoc_fd);
		if (tmp->target)
			free(tmp->target);
		free(tmp);
	}
}

void	remove_empty_tokens(t_token **head)
{
	t_token	*cur;
	t_token	*prev;
	t_token	*next;

	cur = *head;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if (!cur->value || cur->value[0] == '\0')
		{
			if (prev)
				prev->next = next;
			else
				*head = next;
			free(cur->value);
			free(cur);
		}
		else
		{
			prev = cur;
		}
		cur = next;
	}
}

bool	is_heredoc_delim(t_token *token, t_token *prev)
{
	if (!token || !prev)
		return (false);
	return (prev->type == TOKEN_HEREDOC && token->type == TOKEN_WORD);
}

void	remove_quotes_except_heredoc(t_token *tokens)
{
	t_token	*tok;
	t_token	*prev;
	char	*new_val;

	prev = NULL;
	tok = tokens;
	while (tok)
	{
		if (tok->type == TOKEN_WORD && !is_heredoc_delim(tok, prev))
		{
			new_val = remove_quotes(tok->value);
			free(tok->value);
			tok->value = new_val;
		}
		prev = tok;
		tok = tok->next;
	}
}
