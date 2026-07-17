/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 17:42:08 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 18:55:35 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	match_pattern(char *pattern, char *str)
{
	if (!*pattern && !*str)
		return (true);
	if (*pattern == '*')
	{
		while (*str)
		{
			if (match_pattern(pattern + 1, str))
				return (true);
			str++;
		}
		return (match_pattern(pattern + 1, str));
	}
	if (*pattern == '?' || *pattern == *str)
		return (match_pattern(pattern + 1, str + 1));
	return (false);
}

static char	**get_matches(char *pattern, int *count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;

	matches = NULL;
	*count = 0;
	dir = opendir(".");
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
		{
			matches = realloc(matches, sizeof(char *) * (*count + 2));
			if (!matches)
				return (closedir(dir), NULL);
			matches[*count] = ft_strdup(entry->d_name);
			(*count)++;
			matches[*count] = NULL;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (matches);
}

static t_token	*find_token_prev(t_token **head, t_token *old)
{
	t_token	*cur;
	t_token	*prev;

	cur = *head;
	prev = NULL;
	while (cur && cur != old)
	{
		prev = cur;
		cur = cur->next;
	}
	if (!cur)
		return (NULL);
	return (prev);
}

/*
** insert_tokens() replaces one token with a list of new tokens.
** It finds the old token, inserts the matches in its place,
** and frees the old token.
*/
static void	insert_tokens(t_token **head,
			t_token *old, char **matches, int count)
{
	t_token	*prev;
	t_token	*tmp;
	t_token	*new_tok;
	int		i;

	prev = find_token_prev(head, old);
	if (!old)
		return ;
	tmp = old->next;
	i = 0;
	while (i < count)
	{
		new_tok = new_token(ft_strdup(matches[i]), TOKEN_WORD);
		if (!new_tok)
			return ;
		new_tok->next = tmp;
		if (prev)
			prev->next = new_tok;
		else
			*head = new_tok;
		prev = new_tok;
		i++;
	}
	free(old->value);
	free(old);
}

void	expand_wildcards(t_token **tokens)
{
	t_token		*tok;
	t_token		*next;
	int			count;
	char		**matches;

	tok = *tokens;
	while (tok)
	{
		next = tok->next;
		if (tok->type == TOKEN_WORD && tok->status == NO_QUOTE
			&& has_wildcard(tok->value))
		{
			count = 0;
			matches = get_matches(tok->value, &count);
			if (matches && count > 0)
				insert_tokens(tokens, tok, matches, count);
			ft_free_array(matches);
		}
		tok = next;
	}
}
