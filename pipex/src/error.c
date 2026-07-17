/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:42:54 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/11 15:19:20 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(const char *msg)
{
	perror(msg);
	exit(1);
}

void	error_execv(const char *msg)
{
	perror(msg);
	if (errno == ENOENT)
		exit(127);
	else if (errno == EACCES)
		exit(126);
	else
		exit(1);
}

void	safe_exit_child(int fd[2], int infile, const char *msg)
{
	if (fd[0] >= 0)
		close(fd[0]);
	if (fd[1] >= 0)
		close(fd[1]);
	if (infile >= 0)
		close(infile);
	perror(msg);
	exit(EXIT_FAILURE);
}

void	fork_error(int fd1, int fd2)
{
	close_unused_fds(fd1, fd2);
	perror("fork");
	exit(1);
}
