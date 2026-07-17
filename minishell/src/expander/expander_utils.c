/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 19:04:09 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 16:16:24 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_identifier_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	is_var_end(char c)
{
	return (!ft_isalnum(c) && c != '_' && c != '?');
}

char	*get_var_name(char *str)
{
	int		i;
	char	*name;

	if (str[0] == '?')
		return (ft_strdup("?"));
	i = 0;
	while (str[i] && is_valid_identifier_char(str[i]))
		i++;
	name = ft_substr(str, 0, i);
	return (name);
}

char	*str_append_char(char *s, char c)
{
	char	*res;
	int		len;
	int		j;

	len = ft_strlen(s);
	res = malloc(len + 2);
	if (!res)
		return (NULL);
	j = 0;
	while (j < len)
	{
		res[j] = s[j];
		j++;
	}
	res[j] = c;
	res[j + 1] = '\0';
	free(s);
	return (res);
}

char	*str_append_str(char *s1, char *s2)
{
	char	*res;
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	free(s1);
	res = tmp;
	return (res);
}
