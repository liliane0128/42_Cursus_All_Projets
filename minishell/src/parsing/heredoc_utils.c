/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:40:57 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/17 12:20:21 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_quotes(char *str)
{
	int	len;

	if (!str || !*str)
		return (false);
	len = ft_strlen(str);
	return ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '\"' && str[len -1] == '\"'));
}

t_quote_status	get_heredoc_quote(char *s)
{
	int	len;

	if (!s || !*s)
		return (NO_QUOTE);
	len = ft_strlen(s);
	if (s[0] == '\'' && s[len - 1] == '\'')
		return (SINGLE_QUOTE);
	if (s[0] == '\"' && s[len - 1] == '\"')
		return (DOUBLE_QUOTE);
	return (NO_QUOTE);
}

char	*trim_quotes(char *str)
{
	if (!str)
		return (NULL);
	if (has_quotes(str))
		return (ft_substr(str, 1, ft_strlen(str) - 2));
	return (ft_strdup(str));
}

void	close_all_heredoc_fds(t_ast *node)
{
	t_redir	*re;
	t_cmd	*cmd;

	if (!node)
		return ;
	close_all_heredoc_fds(node->left);
	close_all_heredoc_fds(node->right);
	if (!node->cmd)
		return ;
	cmd = node->cmd;
	while (cmd)
	{
		re = cmd->redirs;
		while (re)
		{
			if (re->type == REDIR_HEREDOC && re->heredoc_fd >= 0)
			{
				close(re->heredoc_fd);
				re->heredoc_fd = -1;
			}
			re = re->next;
		}
		cmd = cmd->next;
	}
}
