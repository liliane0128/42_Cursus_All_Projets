/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:41:34 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/13 16:41:34 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				result[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

int	skip_quotes(char *str, int i, char quote)
{
	i++;
	while (str[i] && str[i] != quote)
		i++;
	if (str[i] == quote)
		i++;
	return (i);
}

int	get_token_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (is_operator_char(str[i]))
	{
		if ((str[i] == '<' && str[i + 1] == '<') \
		|| (str[i] == '>' && str[i + 1] == '>') \
		|| (str[i] == '&' && str[i + 1] == '&') \
		|| (str[i] == '|' && str[i + 1] == '|'))
			return (2);
		return (1);
	}
	while (str[i] && !is_operator_char(str[i]) && !is_whitespace(str[i]))
	{
		if (str[i] == '\'' || str[i] == '"')
			i = skip_quotes(str, i, str[i]);
		else
			i++;
	}
	return (i);
}
