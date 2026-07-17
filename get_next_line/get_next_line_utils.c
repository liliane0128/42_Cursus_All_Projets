/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:07:09 by lilwang           #+#    #+#             */
/*   Updated: 2025/05/24 12:40:12 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}

char	*ensure_stash(char *stash)
{
	if (!stash)
	{
		stash = malloc(1);
		if (!stash)
			return (NULL);
		stash[0] = '\0';
	}
	return (stash);
}

char	*ft_strjoin(char *stash, char *buff)
{
	size_t	i;
	size_t	j;
	char	*str;

	if (!stash || !buff)
		return (free(stash), NULL);
	str = malloc(ft_strlen(stash) + ft_strlen(buff) + 1);
	if (!str)
	{
		free(stash);
		return (NULL);
	}
	i = -1;
	while (stash[++i])
		str[i] = stash[i];
	j = 0;
	while (buff[j])
		str[i++] = buff[j++];
	str[i] = '\0';
	free(stash);
	return (str);
}
