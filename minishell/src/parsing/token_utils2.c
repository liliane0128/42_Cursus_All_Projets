/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 11:46:31 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/19 11:57:43 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_syntax_tokens(t_token *tok, t_data *data)
{
	t_token	*prev;
	t_token	*cur;
	t_token	*next;

	prev = NULL;
	cur = tok;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOKEN_OR || cur->type == TOKEN_AND)
		{
			if (!prev || !next)
				return (true);
			if ((prev->type != TOKEN_WORD && prev->type != TOKEN_RPAREN)
				|| (next->type != TOKEN_WORD && next->type != TOKEN_LPAREN))
			{
				errmsg_cmd("minishell", cur->value, ERR_MSG, 2);
				data->last_status = 2;
				return (false);
			}
		}
		prev = cur;
		cur = cur->next;
	}
	return (true);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '&' || c == '<' || c == '>' \
		|| c == '(' || c == ')');
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}
