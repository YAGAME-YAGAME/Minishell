/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 08:03:07 by yagame            #+#    #+#             */
/*   Updated: 2025/05/25 10:16:44 by otzarwal         ###   ########.fr       */
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


void	handle_parent_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	g_exit_status = 130;
}


void	handle_parent_heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	g_exit_status = 1;
}

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	exit(1);
}

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

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_parent_waiting_signals(void)
{
	signal(SIGINT, handle_parent_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_parent_heredoc_signals(void)
{
	signal(SIGINT, handle_parent_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}
