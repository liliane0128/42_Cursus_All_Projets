/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 14:46:58 by lilwang           #+#    #+#             */
/*   Updated: 2025/05/24 15:09:10 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif

char	*get_next_line(int fd);
char	*fill_buffer_util_newline(int fd, char *stash);
char	*extract_line(char *stash);
char	*get_new_buffer(char *stash);
size_t	ft_strlen(const char *s);
char	*extract_line(char *stash);
char	*ensure_stash(char *stash);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char *stash, char *buff);

#endif
