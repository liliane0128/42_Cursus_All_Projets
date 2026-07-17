/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:40:44 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/19 21:00:18 by shazhu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->av = NULL;
	cmd->toklist = NULL;
	cmd->redirs = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->head = NULL;
	return (cmd);
}

bool	is_cmd_end(t_token *token)
{
	if (!token)
		return (true);
	return (token->type == TOKEN_PIPE
		|| token->type == TOKEN_AND
		|| token->type == TOKEN_OR
		|| token->type == TOKEN_RPAREN);
}

int	count_args(t_token *tokens)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = tokens;
	while (tmp && tmp->type != TOKEN_PIPE && tmp->type != TOKEN_AND \
			&& tmp->type != TOKEN_OR && tmp->type != TOKEN_RPAREN)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		else if (tmp->type >= TOKEN_REDIR_IN && tmp->type <= TOKEN_HEREDOC)
		{
			if (tmp->next)
				tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	return (count);
}

/*
** handle_token_arg() returns a string if the token is a word,
** or skips the token if it's a redirection. Advances the token pointer.
*/
static char	*handle_token_arg(t_token **tokens)
{
	char	*val;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_WORD)
	{
		val = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		return (val);
	}
	else if ((*tokens)->type >= TOKEN_REDIR_IN
		&& (*tokens)->type <= TOKEN_HEREDOC)
	{
		*tokens = (*tokens)->next;
		if (*tokens)
			*tokens = (*tokens)->next;
		return (NULL);
	}
	else
		return (NULL);
}

/*
** extract_args() collects all TOKEN_WORD values into a NULL-terminated array.
** Stops at PIPE, AND, OR tokens or end of list.
*/
char	**extract_args(t_token **tokens)
{
	char	**args;
	int		i;
	int		count;
	char	*val;

	count = count_args(*tokens);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE \
			&& (*tokens)->type != TOKEN_AND && (*tokens)->type != TOKEN_OR \
			&& (*tokens)->type != TOKEN_RPAREN)
	{
		val = handle_token_arg(tokens);
		if (val)
			args[i++] = val;
		else if (!*tokens || (*tokens)->type == TOKEN_PIPE \
				|| (*tokens)->type == TOKEN_AND \
				|| (*tokens)->type == TOKEN_OR \
				|| (*tokens)->type == TOKEN_RPAREN)
			break ;
	}
	args[i] = NULL;
	return (args);
}
