/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:41:26 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 18:24:35 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*copy_tokens(t_token *head)
{
	t_token	*new_head;
	t_token	*prev;
	t_token	*cur;
	t_token	*new;

	new_head = NULL;
	prev = NULL;
	cur = head;
	while (cur)
	{
		new = malloc(sizeof(t_token));
		if (!new)
			return (free_tokens(new_head), NULL);
		new->type = cur->type;
		new->status = cur->status;
		new->value = ft_strdup(cur->value);
		new->next = NULL;
		if (!new_head)
			new_head = new;
		if (prev)
			prev->next = new;
		prev = new;
		cur = cur->next;
	}
	return (new_head);
}

t_cmd	*parse_simple_cmd(t_token **tokens, t_data *data)
{
	t_token	*start;

	if (!tokens || !*tokens)
		return (NULL);
	data->cmd = init_cmd();
	if (!data->cmd)
		return (NULL);
	if (!parse_redirections(data->cmd, tokens, data))
	{
		data->last_status = 1;
		return (free_single_cmd(data->cmd), data->cmd = NULL, NULL);
	}
	start = *tokens;
	data->cmd->toklist = copy_tokens(start);
	data->cmd->av = extract_args(&start);
	*tokens = start;
	while (*tokens && (*tokens)->type != TOKEN_PIPE
		&& (*tokens)->type != TOKEN_AND
		&& (*tokens)->type != TOKEN_OR
		&& (*tokens)->type != TOKEN_RPAREN)
		*tokens = (*tokens)->next;
	return (data->cmd);
}

t_ast	*parse(char *input, t_data *data)
{
	t_token	*tokens;
	t_ast	*root;

	if (!input || !*input)
		return (NULL);
	tokens = tokenize(input);
	if (!tokens)
		return (NULL);
	remove_empty_tokens(&tokens);
	if (!check_syntax_tokens(tokens, data) || !check_redir_tokens(tokens, data)
		|| !check_empty_pipe(tokens, data))
	{
		free_tokens(tokens);
		return (NULL);
	}
	root = build_ast(tokens, data);
	free_tokens(tokens);
	return (root);
}
