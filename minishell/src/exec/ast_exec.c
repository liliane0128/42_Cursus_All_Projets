/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shazhu <shazhu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 15:38:29 by shazhu            #+#    #+#             */
/*   Updated: 2025/10/21 15:38:29 by shazhu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_logical(t_ast *node, t_data *data)
{
	int	status;

	status = exec_ast(node->left, data);
	if ((node->type == NODE_AND && status == 0)
		|| (node->type == NODE_OR && status != 0))
		status = exec_ast(node->right, data);
	return (status);
}

static int	exec_subshell_child(t_ast *node, t_data *data)
{
	int	status;

	if (!handle_redirs(node->cmd))
	{
		cleanup(data);
		exit(1);
	}
	status = exec_ast(node->subshell, data);
	cleanup(data);
	exit(status);
}

static int	exec_subshell(t_ast *node, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		exec_subshell_child(node, data);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

int	exec_ast(t_ast *node, t_data *data)
{
	if (!node)
		return (0);
	if (!preprocess_heredocs(node, data))
		return (data->last_status);
	if (node->type == NODE_CMD)
		return (exec_cmd_node(node, data));
	else if (node->type == NODE_PIPE)
		return (exec_pipe_ast(node, data));
	else if (node->type == NODE_AND || node->type == NODE_OR)
		return (exec_logical(node, data));
	else if (node->type == NODE_SUBSHELL)
		return (exec_subshell(node, data));
	return (0);
}
