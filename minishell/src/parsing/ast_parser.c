/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:40:30 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/19 20:54:21 by shazhu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** parse_single_command: parse a single command into a CMD AST node
** Returns a new AST node or NULL on failure
*/
static t_ast	*parse_single_command(t_token **tokens, t_data *data)
{
	t_cmd	*cmd;
	t_ast	*node;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_AND || (*tokens)->type == TOKEN_OR)
		return (NULL);
	cmd = parse_simple_cmd(tokens, data);
	if (!cmd)
		return (NULL);
	node = new_ast_node(NODE_CMD, cmd);
	if (!node)
		return (free_cmds(cmd), NULL);
	return (node);
}

t_ast	*parse_factor(t_token **tokens, t_data *data)
{
	t_ast	*node;
	t_ast	*sub_node;

	if (*tokens && (*tokens)->type == TOKEN_LPAREN)
	{
		*tokens = (*tokens)->next;
		sub_node = parse_expr(tokens, data);
		if (!sub_node || !*tokens || (*tokens)->type != TOKEN_RPAREN)
			return (free_ast(sub_node), NULL);
		*tokens = (*tokens)->next;
		node = new_ast_node(NODE_SUBSHELL, NULL);
		if (!node)
		{
			free_ast(sub_node);
			return (NULL);
		}
		node->subshell = sub_node;
	}
	else
	{
		node = parse_single_command(tokens, data);
		if (!node)
			return (NULL);
	}
	return (node);
}

t_ast	*parse_pipeline(t_token **tokens, t_data *data)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe;

	left = parse_factor(tokens, data);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_factor(tokens, data);
		if (!right)
			return (free_ast(left), NULL);
		pipe = new_ast_node(NODE_PIPE, NULL);
		if (!pipe)
			return (free_ast(left), free_ast(right), NULL);
		pipe->left = left;
		pipe->right = right;
		left = pipe;
	}
	return (left);
}

t_ast	*parse_expr(t_token **tokens, t_data *data)
{	
	t_ast		*left;
	t_ast		*right;
	t_ast		*node;
	t_node_type	type;

	left = parse_pipeline(tokens, data);
	if (!left)
		return (NULL);
	while (*tokens && ((*tokens)->type == TOKEN_AND \
				|| (*tokens)->type == TOKEN_OR))
	{
		type = get_logic_type(*tokens);
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens, data);
		if (!right)
			return (free_ast(left), NULL);
		node = new_ast_node(type, NULL);
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

t_ast	*build_ast(t_token *tokens, t_data *data)
{
	if (!tokens)
		return (NULL);
	return (parse_expr(&tokens, data));
}
