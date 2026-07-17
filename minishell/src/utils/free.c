/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:30:43 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/19 15:04:18 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_array(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_single_cmd(t_cmd	*cmd)
{
	if (!cmd)
		return ;
	if (cmd->av)
	{
		ft_free_array(cmd->av);
		cmd->av = NULL;
	}	
	if (cmd->redirs)
	{
		free_redirs(cmd->redirs);
		cmd->redirs = NULL;
	}
	if (cmd->toklist)
	{
		free_tokens(cmd->toklist);
		cmd->toklist = NULL;
	}
	free(cmd);
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_single_cmd(cmd);
		cmd = tmp;
	}
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_SUBSHELL)
		free_ast(node->subshell);
	if (node->type == NODE_PIPE || node->type == NODE_AND \
		|| node->type == NODE_OR)
	{
		free_ast(node->left);
		free_ast(node->right);
	}
	if (node->type == NODE_CMD && node->cmd)
	{
		free_single_cmd(node->cmd);
	}
	free(node);
}

void	cleanup(t_data *data)
{
	if (!data)
		return ;
	if (data)
	{
		if (data->ast)
		{
			close_all_heredoc_fds(data->ast);
			free_ast(data->ast);
			data->ast = NULL;
		}
		if (data->envp)
			ft_free_array(data->envp);
		if (data->saved_stdin != -1)
			close(data->saved_stdin);
		if (data->saved_stdout != -1)
			close(data->saved_stdout);
	}
}
