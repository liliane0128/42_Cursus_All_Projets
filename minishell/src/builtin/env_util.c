/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 17:30:08 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 14:57:36 by shazhu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_envp_dup(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i-- > 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static int	ft_env_keycmp(const char *a, const char *b)
{
	size_t	len_a;
	size_t	len_b;
	size_t	min_len;
	int		res;

	len_a = 0;
	len_b = 0;
	while (a[len_a] && a[len_a] != '=')
		len_a++;
	while (b[len_b] && b[len_b] != '=')
		len_b++;
	if (len_a < len_b)
		min_len = len_a;
	else
		min_len = len_b;
	res = ft_strncmp(a, b, min_len);
	if (res == 0)
		return ((int)(len_a - len_b));
	return (res);
}

static void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

char	**ft_sort_env(char **envp)
{
	char	**sorted;
	int		i;
	int		j;

	sorted = ft_envp_dup(envp);
	if (!sorted)
		return (NULL);
	i = 0;
	while (sorted[i])
	{
		j = i + 1;
		while (sorted[j])
		{
			if (ft_env_keycmp(sorted[i], sorted[j]) > 0)
				swap(&sorted[i], &sorted[j]);
			j++;
		}
		i++;
	}
	return (sorted);
}

char	**ft_realloc_env(char **envp, const char *new_var)
{
	int		len;
	char	**new_env;
	int		i;

	len = 0;
	while (envp[len])
		len++;
	new_env = malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_env[i] = envp[i];
		i++;
	}
	new_env[i] = ft_strdup(new_var);
	if (!new_env[i])
	{
		free(new_env);
		return (NULL);
	}
	new_env[i + 1] = NULL;
	free(envp);
	return (new_env);
}
