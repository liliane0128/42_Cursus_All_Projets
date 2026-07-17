/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 18:58:40 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/11 14:03:17 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_unused_fds(int fd1, int fd2)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
}

static void	child1(int fd[2], char **av, char **env)
{
	int	infile;

	infile = open(av[1], O_RDONLY);
	if (infile < 0)
	{
		perror("infile");
		close_unused_fds(fd[0], fd[1]);
		exit(1);
	}
	if (dup2(infile, STDIN_FILENO) == -1)
		safe_exit_child(fd, infile, "dup2 infile");
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		safe_exit_child(fd, infile, "dup2 pipe write");
	close_unused_fds(fd[0], fd[1]);
	if (infile >= 0)
		close(infile);
	execute_cmd(av[2], env);
	exit(EXIT_FAILURE);
}

static void	child2(int fd[2], char **av, char **env)
{
	int	outfile;

	outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
	{
		close_unused_fds(fd[0], fd[1]);
		error_exit("outfile open");
	}
	if (dup2(fd[0], STDIN_FILENO) == -1)
		safe_exit_child(fd, outfile, "dup2 infile");
	if (dup2(outfile, STDOUT_FILENO) == -1)
		safe_exit_child(fd, outfile, "dup2 pipe write");
	close_unused_fds(fd[0], fd[1]);
	if (outfile >= 0)
		close(outfile);
	execute_cmd(av[3], env);
	exit(EXIT_FAILURE);
}

static int	do_pipex(char **av, char **env)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		exit_code;

	if (pipe(fd) == -1)
		error_exit("pipe");
	pid1 = fork();
	if (pid1 == -1)
		fork_error(fd[0], fd[1]);
	if (pid1 == 0)
		child1(fd, av, env);
	pid2 = fork();
	if (pid2 == -1)
		fork_error(fd[0], fd[1]);
	if (pid2 == 0)
		child2(fd, av, env);
	close_unused_fds(fd[0], fd[1]);
	exit_code = wait_all_pids(pid2);
	return (exit_code);
}

int	main(int ac, char **av, char **env)
{
	int	exit_code;

	if (ac != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		exit(1);
	}
	exit_code = do_pipex(av, env);
	return (exit_code);
}
