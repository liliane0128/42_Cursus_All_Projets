/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:49:53 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 18:25:03 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_builtin(t_cmd *cmd, t_data *data)
{
	int	ret;

	ret = CMD_NOT_FOUND;
	if (ft_strcmp(cmd->av[0], "clear") == 0)
	{
		write(1, "\033[H\033[J", 6);
		ret = 0;
	}
	if (ft_strcmp(cmd->av[0], "cd") == 0)
		ret = cd_builtin(data, cmd->av);
	else if (ft_strcmp(cmd->av[0], "echo") == 0)
		ret = echo_builtin(data, cmd->av);
	else if (ft_strcmp(cmd->av[0], "env") == 0)
		ret = env_builtin(data, cmd->av);
	else if (ft_strcmp(cmd->av[0], "export") == 0)
		ret = export_builtin(data, cmd->av);
	else if (ft_strcmp(cmd->av[0], "pwd") == 0)
		ret = pwd_builtin(data, cmd->av);
	else if (ft_strcmp(cmd->av[0], "unset") == 0)
		ret = unset_builtin(data, cmd->av);
	else if (ft_strcmp(cmd->av[0], "exit") == 0)
		ret = exit_builtin(data, cmd->av);
	return (ret);
}

static int	execute_builtin(t_cmd *cmd, t_data *data)
{
	int	ret;

	if (!save_stdio(&data->saved_stdin, &data->saved_stdout))
	{
		perror("minishell");
		return (EXIT_FAILURE);
	}
	if (!handle_redirs(cmd))
	{
		restore_stdio(data->saved_stdin, data->saved_stdout);
		data->saved_stdin = -1;
		data->saved_stdout = -1;
		data->last_status = 1;
		return (EXIT_FAILURE);
	}
	ret = run_builtin(cmd, data);
	restore_stdio(data->saved_stdin, data->saved_stdout);
	data->saved_stdin = -1;
	data->saved_stdout = -1;
	close_redirs(cmd->redirs);
	return (ret);
}

static int	run_external_child(t_cmd *cmd, t_data *data)
{
	char	*path;

	if (!cmd || !cmd->av || !cmd->av[0])
		return (0);
	if (!handle_redirs(cmd))
		cleanup_and_exit(1, data, NULL, 0);
	close_all_heredoc_fds(data->ast);
	path = find_command_path(cmd->av[0], data->envp);
	if (!path)
	{
		if (cmd_contains_slash(cmd->av[0]))
			cleanup_and_exit(126, data, NULL, 0);
		errmsg_cmd(cmd->av[0], NULL, "command not found", 127);
		close_redirs(cmd->redirs);
		cleanup_and_exit(127, data, NULL, 0);
	}
	execve(path, cmd->av, data->envp);
	close_redirs(cmd->redirs);
	execve_error(data, path);
	free(path);
	return (0);
}

static int	execute_external(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	ignore_signals();
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		setup_child_signals();
		run_external_child(cmd, data);
		cleanup_and_exit(1, data, NULL, 0);
	}
	setup_parent_signals();
	if (waitpid(pid, &status, 0) < 0)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status))
		data->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		data->last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "QUIT (core dumped)\n", 19);
	}
	return (data->last_status);
}

int	exec_cmd_node(t_ast *node, t_data *data)
{
	t_token	*temp_toklist;

	if (!node || !node->cmd)
		return (0);
	data->cmd = node->cmd;
	expand_tokens(data, node->cmd->toklist);
	remove_empty_tokens(&node->cmd->toklist);
	remove_quotes_except_heredoc(node->cmd->toklist);
	expand_wildcards(&node->cmd->toklist);
	if (node->cmd->av)
		ft_free_array(node->cmd->av);
	temp_toklist = node->cmd->toklist;
	node->cmd->av = extract_args(&temp_toklist);
	if ((!node->cmd->av || !node->cmd->av[0] || node->cmd->av[0][0] == '\0'))
	{
		handle_redirections_only(node->cmd->toklist, data);
		return (data->last_status);
	}
	if (is_builtin(node->cmd->av[0]))
		data->last_status = execute_builtin(node->cmd, data);
	else
		data->last_status = execute_external(node->cmd, data);
	return (data->last_status);
}
