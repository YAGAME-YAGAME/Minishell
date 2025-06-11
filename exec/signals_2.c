/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 23:08:48 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Sets up signal handlers for heredoc input collection.
 * Configures SIGINT to use the heredoc-specific handler and ignores
 * SIGQUIT. Used when child processes are collecting heredoc input
 * to ensure proper termination on interrupt signals.
 *
 * Side effects: Modifies signal handlers for SIGINT and SIGQUIT
 */
void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Restores default interactive shell signal handlers.
 * Resets signal handlers back to the main shell configuration
 * after special signal handling contexts (like heredoc or child waiting)
 * are complete. Restores normal interactive behavior.
 *
 * Side effects: Modifies signal handlers for SIGINT and SIGQUIT
 */
void	restore_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Sets up default signal handlers for child processes.
 * Configures child processes to use default signal behavior
 * for SIGINT and SIGQUIT instead of the custom shell handlers.
 * This allows child commands to be interrupted normally.
 *
 * Side effects: Modifies signal handlers for SIGINT and SIGQUIT
 */
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/*
 * Sets up signal handlers for parent process while waiting for children.
 * Configures specific signal handling when the parent process is
 * waiting for child processes to complete. Uses specialized handlers
 * to manage interrupts during command execution.
 *
 * Side effects: Modifies signal handlers for SIGINT and SIGQUIT
 */
void	setup_parent_waiting_signals(void)
{
	signal(SIGINT, handle_parent_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Sets up signal handlers for parent process during heredoc processing.
 * Configures signal handling when the parent is coordinating heredoc
 * input collection. Uses specialized handlers to manage interrupts
 * during heredoc operations.
 *
 * Side effects: Modifies signal handlers for SIGINT and SIGQUIT
 */
void	setup_parent_heredoc_signals(void)
{
	signal(SIGINT, handle_parent_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}
