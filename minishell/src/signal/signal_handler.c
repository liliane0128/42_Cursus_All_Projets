/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 17:14:01 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/19 14:18:02 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_last_code = 128 + SIGINT;
}

void	sigint_heredoc(int sig)
{
	(void)sig;
	g_last_code = 130;
	rl_done = 1;
	rl_replace_line("", 0);
}

void	sigint_handler_multiline(int sig)
{
	(void)sig;
	g_last_code = 128 + SIGINT;
}

void	restore_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_handler_multiline;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

int	ctrlc_event_hook(void)
{
	if (g_last_code == 130)
	{
		rl_done = 1;
		return (0);
	}
	return (0);
}
