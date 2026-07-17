/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 11:49:47 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/27 12:31:19 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

void	ft_free_all(char **split)
{
	size_t	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

char	**ft_fill_split(char **split, char const *s, char c, int start)
{
	int	i;
	int	index;

	i = 0;
	index = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			split[index] = ft_substr(s, start, i - start);
			if (!split[index])
			{
				ft_free_all(split);
				return (NULL);
			}
			index++;
		}
		else
			i++;
	}
	split[index] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		start;

	start = 0;
	if (!s)
		return (NULL);
	split = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!split)
		return (NULL);
	split = ft_fill_split(split, s, c, start);
	if (!split)
		return (NULL);
	return (split);
}
