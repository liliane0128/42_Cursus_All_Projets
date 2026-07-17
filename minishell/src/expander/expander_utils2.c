/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:40:19 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 10:44:35 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_quote_status(t_token *token, char c)
{
	if (c == '\'' && token->status != DOUBLE_QUOTE)
	{
		if (token->status == SINGLE_QUOTE)
			token->status = NO_QUOTE;
		else
			token->status = SINGLE_QUOTE;
	}
	else if (c == '\"' && token->status != SINGLE_QUOTE)
	{
		if (token->status == DOUBLE_QUOTE)
			token->status = NO_QUOTE;
		else
			token->status = DOUBLE_QUOTE;
	}
}

char	*expand_variable_value(t_token *tok, int *i, t_data *data)
{
	char	*var;
	char	*val;
	int		start;

	start = *i + 1;
	if (tok->value[start] == '?')
	{
		val = ft_itoa(data->last_status);
		*i = start + 1;
		return (val);
	}
	var = get_var_name(tok->value + start);
	if (!var)
		return (ft_strdup(""));
	val = get_env_value(data->envp, var);
	*i = start + ft_strlen(var);
	free(var);
	return (val);
}

bool	has_wildcard(char *s)
{
	while (*s)
	{
		if (*s == '*' || *s == '?')
			return (true);
		s++;
	}
	return (false);
}
