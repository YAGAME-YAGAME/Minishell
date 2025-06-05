/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 08:03:07 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 15:15:06 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Handles SIGINT signal during interactive shell mode.
 * When Ctrl+C is pressed in the shell prompt, this handler clears the
 * current line, moves to a new line, and redisplays the prompt.
 * Sets the global exit status to 1 to indicate interruption.
 *
 * @param sig: Signal number (unused)
 * Side effects: Writes newline, clears readline buffer, redisplays prompt,
 * sets exit status
 */
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

/*
 * Handles SIGINT signal when parent process is waiting for child execution.
 * Used during command execution when the parent process needs to handle
 * interrupts differently than in interactive mode. Simply prints a newline
 * and sets the exit status to 130 (128 + SIGINT).
 *
 * @param sig: Signal number (unused)
 * Side effects: Writes newline, sets exit status to 130
 */
void	handle_parent_sigint(int sig)
{
	(void)sig;
	g_exit_status = 130;
}

/*
 * Handles SIGINT signal during heredoc input collection.
 * When Ctrl+C is pressed while collecting heredoc input, this handler
 * prints a newline and sets the exit status to 1, allowing graceful
 * termination of the heredoc input process.
 *
 * @param sig: Signal number (unused)
 * Side effects: Writes newline, sets exit status to 1
 */
void	handle_parent_heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	g_exit_status = 1;
}

/*
 * Handles SIGINT signal in child processes during heredoc input.
 * Child processes handling heredoc input should exit immediately
 * when interrupted, so this handler simply exits with status 1.
 *
 * @param sig: Signal number (unused)
 * Side effects: Exits child process with status 1
 */
void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	exit(1);
}

/*
 * Sets up signal handlers for interactive shell mode.
 * Configures SIGINT to use the interactive handler and ignores SIGQUIT
 * to prevent core dumps when Ctrl+\ is pressed. This is the default
 * signal configuration for the shell prompt.
 *
 * Side effects: Modifies signal handlers for SIGINT and SIGQUIT
 */
void	handle_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
