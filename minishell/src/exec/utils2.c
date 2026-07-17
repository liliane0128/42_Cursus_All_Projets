/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 13:43:36 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/21 10:43:42 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	cmd_contains_slash(char *cmd)
{
	if (!cmd)
		return (false);
	while (*cmd)
	{
		if (*cmd == '/')
			return (true);
		cmd++;
	}
	return (false);
}

static void	process_redir(t_token *cur, t_data *data)
{
	int	fd;

	if (cur->type == TOKEN_REDIR_OUT)
		fd = open(cur->next->value, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (cur->type == TOKEN_APPEND)
		fd = open(cur->next->value, O_CREAT | O_APPEND | O_WRONLY, 0644);
	else
	{
		if (!cur->next || !cur->next->value)
			return ;
		fd = open(cur->next->value, O_RDONLY);
	}
	if (fd < 0)
		data->last_status = errmsg_cmd("minishell: ", cur->next->value,
				strerror(errno), 1);
	else
		close(fd);
}

void	handle_redirections_only(t_token *toklist, t_data *data)
{
	t_token	*cur;

	cur = toklist;
	while (cur)
	{
		if ((cur->type == TOKEN_REDIR_OUT || cur->type == TOKEN_APPEND
				|| cur->type == TOKEN_REDIR_IN)
			&& cur->next && cur->next->value)
			process_redir(cur, data);
		cur = cur->next;
	}
}
