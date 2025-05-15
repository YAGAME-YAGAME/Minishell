/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 08:03:07 by yagame            #+#    #+#             */
/*   Updated: 2025/05/09 16:16:40 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	printf("");
	g_exit_status = 1;
}

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	exit(1);
}

// void handle_heredoc_sigint(int sig)
// {
//     (void)sig;
//     write(1, "\n", 1);
//     g_exit_status = 1;
//     exit(1);
// }
void	handle_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
