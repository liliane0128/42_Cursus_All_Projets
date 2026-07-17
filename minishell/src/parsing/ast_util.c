/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 10:47:21 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 16:40:38 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** AST (Abstract Syntax Tree) for minishell
**
** Represents commands, pipelines, and logical operators (&&, ||).
** Used to:
** 1. Build the command structure from tokens
** 2. Preserve execution order and operator precedence
** 3. Execute commands recursively
**
** Example:
**   ls | grep txt && echo done
**   AST:
**          AND
**         /   \
**      PIPE   CMD(echo done)
**     /    \
** CMD(ls) CMD(grep txt)
*/
t_ast	*new_ast_node(t_node_type type, t_cmd *cmd)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	node->subshell = NULL;
	return (node);
}

t_node_type	get_logic_type(t_token *token)
{
	if (token->type == TOKEN_AND)
		return (NODE_AND);
	if (token->type == TOKEN_OR)
		return (NODE_OR);
	return (NODE_CMD);
}
