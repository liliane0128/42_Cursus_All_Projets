/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:07:38 by lilwang           #+#    #+#             */
/*   Updated: 2025/05/24 12:08:50 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
size_t	ft_strlen(char *str);
char	*ft_strchr(const char *s, int c);
char	*ensure_stash(char *stash);
char	*ft_strjoin(char *stash, char *buff);
char	*extract_line(char *stash);
char	*get_new_buffer(char *stash);
char	*fill_buffer_util_newline(int fd, char *stash);

#endif
