/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:56:50 by lilwang           #+#    #+#             */
/*   Updated: 2025/08/11 14:02:49 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <fcntl.h>
# include "libft.h"

void	error_exit(const char *msg);
void	execute_cmd(char *cmd, char **env);
void	ft_free_split(char **split);
char	*get_path(char *cmd, char **env);
int		wait_all_pids(pid_t last_pid);
void	error_execv(const char *msg);
void	safe_exit_child(int fd[2], int infile, const char *msg);
void	close_unused_fds(int fd1, int fd2);
void	fork_error(int fd1, int fd2);

#endif