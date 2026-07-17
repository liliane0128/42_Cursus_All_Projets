/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 19:55:26 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/10 19:55:26 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_operator_type(char *str)
{
	if (str[0] == '|' && str[1] == '|')
		return (TOKEN_OR);
	if (str[0] == '|')
		return (TOKEN_PIPE);
	if (str[0] == '&' && str[1] == '&')
		return (TOKEN_AND);
	if (str[0] == '(')
		return (TOKEN_LPAREN);
	if (str[0] == ')')
		return (TOKEN_RPAREN);
	if (str[0] == '<' && str[1] == '<')
		return (TOKEN_HEREDOC);
	if (str[0] == '<')
		return (TOKEN_REDIR_IN);
	if (str[0] == '>' && str[1] == '>')
		return (TOKEN_APPEND);
	if (str[0] == '>')
		return (TOKEN_REDIR_OUT);
	return (TOKEN_WORD);
}

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	token->status = NO_QUOTE;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static t_token	*create_new_token(char *value, char *input, int i)
{
	t_token	*token;

	if (!value || value[0] == '\0')
	{
		token = new_token(ft_strdup(""), TOKEN_WORD);
		if (!token)
			return (NULL);
		return (token);
	}
	if (is_operator_char(input[i]))
		token = new_token(value, get_operator_type(input + i));
	else
		token = new_token(value, TOKEN_WORD);
	if (!token)
		return (free(value), NULL);
	return (token);
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	t_token	*new;
	char	*value;
	int		i;
	int		len;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break ;
		len = get_token_len(input + i);
		value = ft_substr(input, i, len);
		if (!value)
			return (free_tokens(tokens), NULL);
		new = create_new_token(value, input, i);
		if (!new)
			return (free_tokens(tokens), NULL);
		add_token(&tokens, new);
		i += len;
	}
	return (tokens);
}
