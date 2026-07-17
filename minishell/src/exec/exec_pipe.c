/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 13:18:44 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 20:12:13 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_all_children(pid_t last_pid)
{
	pid_t	pid;
	int		status;
	int		exit_code;

	exit_code = 1;
	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
		pid = waitpid(-1, &status, 0);
	}
	return (exit_code);
}

static void	run_left_writer(t_ast *left, int write_end, t_data *data)
{
	int	fds[1];

	fds[0] = write_end;
	if (dup2(write_end, STDOUT_FILENO) < 0)
		cleanup_and_exit(1, data, fds, 1);
	close(write_end);
	cleanup_and_exit(exec_ast(left, data), data, fds, 1);
}

static void	run_right_reader(t_ast *right, int read_end, t_data *data)
{
	int	fds[1];

	fds[0] = read_end;
	if (dup2(read_end, STDIN_FILENO) < 0)
		cleanup_and_exit(1, data, fds, 1);
	close(read_end);
	cleanup_and_exit(exec_ast(right, data), data, fds, 1);
}

static void	setup_children(t_ast *node, int fds[2], t_data *data, pid_t pids[2])
{
	pids[0] = fork();
	if (pids[0] == 0)
	{
		setup_child_signals();
		close(fds[0]);
		run_left_writer(node->left, fds[1], data);
	}
	pids[1] = fork();
	if (pids[1] == 0)
	{
		setup_child_signals();
		close(fds[1]);
		run_right_reader(node->right, fds[0], data);
	}
}

int	exec_pipe_ast(t_ast *node, t_data *data)
{
	int		fds[2];
	pid_t	pids[2];

	if (!node || !node->left || !node->right)
		return (1);
	if (pipe(fds) < 0)
		return (perror("pipe"), 1);
	setup_children(node, fds, data, pids);
	close(fds[0]);
	close(fds[1]);
	data->last_status = wait_all_children(pids[1]);
	setup_parent_signals();
	return (data->last_status);
}
