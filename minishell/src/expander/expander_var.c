/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 16:05:54 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/17 14:59:05 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_dollar_expansion(t_data *data, t_token *tok,
				char **result, int *i)
{
	char	*val;
	int		start;

	if (tok->value[*i + 1] == '?')
	{
		val = ft_itoa(data->last_status);
		(*i) += 2;
	}
	else
	{
		start = *i + 1;
		val = expand_variable_value(tok, i, data);
		if (!val)
			val = ft_strdup("");
	}
	*result = str_append_str(*result, val);
	free(val);
}

/*
** expand_string() expands environment variables in the input line.
** It respects quote rules and skips expansion inside single quotes.
*/
static char	*expand_string(t_data *data, char *line)
{
	int		i;
	t_token	tok;
	char	*result;

	if (!line)
		return (NULL);
	tok.value = ft_strdup(line);
	tok.status = 0;
	result = ft_strdup("");
	i = 0;
	while (tok.value[i])
	{
		update_quote_status(&tok, tok.value[i]);
		if (tok.value[i] == '$' && tok.status != SINGLE_QUOTE
			&& tok.value[i + 1] && !is_var_end(tok.value[i + 1]))
		{
			handle_dollar_expansion(data, &tok, &result, &i);
			continue ;
		}
		result = str_append_char(result, tok.value[i]);
		i++;
	}
	free(tok.value);
	return (result);
}

char	*expand_token_value(t_token *tok, t_data *data)
{
	return (expand_string(data, tok->value));
}

char	*expand_heredoc(t_data *data, char *line)
{
	return (expand_string(data, line));
}

void	expand_tokens(t_data *data, t_token *toklist)
{
	t_token	*cur;
	char	*new_val;

	cur = toklist;
	while (cur)
	{
		if (cur->type == TOKEN_WORD)
		{
			new_val = expand_token_value(cur, data);
			free(cur->value);
			cur->value = new_val;
		}
		cur = cur->next;
	}
}
