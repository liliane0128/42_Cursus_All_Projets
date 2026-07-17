/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:58:33 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/11 14:36:25 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
/*
** execute_cmd:
**  Executes a shell command string.
**  Steps:
**   - Split command into args using ft_split.
**   - Find executable path with get_cmd_path.
**   - If not found, print error and exit(127).
**   - Call execve to run the command.
**   - On execve failure, free resources and exit with error.
**
** get_cmd_path:
**  Finds the full executable path of a command.
**  Steps:
**   - Locate PATH in env.
**   - Split PATH by ':' into dirs.
**   - Check each dir + cmd for existence and executable permission.
**   - Return first valid path or NULL.
*/

void	ft_free_split(char **split)
{
	size_t	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
		free(split[i++]);
	free (split);
}

static void	ft_put_cmd_not_found(char *cmd)
{
	ft_putstr_fd("command not found: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putchar_fd('\n', 2);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**paths;
	char	*path;
	char	*res;

	while (*env && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	if (!*env)
		return (NULL);
	paths = ft_split(*env + 5, ':');
	i = -1;
	while (paths && paths[++i])
	{
		path = ft_strjoin(paths[i], "/");
		res = ft_strjoin(path, cmd);
		free(path);
		if (access(res, F_OK | X_OK) == 0)
		{
			ft_free_split(paths);
			return (res);
		}
		free(res);
	}
	ft_free_split(paths);
	return (NULL);
}

void	execute_cmd(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	if (!s_cmd || !s_cmd[0])
	{
		ft_free_split(s_cmd);
		ft_putstr_fd("pipex: command not found\n", 2);
		exit(127);
	}
	if (ft_strchr(s_cmd[0], '/'))
		path = ft_strdup(s_cmd[0]);
	else
		path = get_path(s_cmd[0], env);
	if (!path)
	{
		ft_put_cmd_not_found(s_cmd[0]);
		ft_free_split(s_cmd);
		exit(127);
	}
	execve(path, s_cmd, env);
	ft_free_split(s_cmd);
	free(path);
	error_execv(cmd);
}

int	wait_all_pids(pid_t last_pid)
{
	int		status;
	int		exit_code;
	pid_t	pid;

	exit_code = 1;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
	return (exit_code);
}
