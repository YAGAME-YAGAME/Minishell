/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiguous.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:34:26 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Checks if a filename contains spaces that would make redirection ambiguous.
 * Detects cases where variable expansion or other operations result in
 * filenames containing spaces, which creates ambiguous redirection in shell.
 * Returns true for empty/null strings and strings containing spaces.
 *
 * @param file: Filename string to check for ambiguity
 * @return: true if filename is ambiguous (empty or contains spaces), false otherwise
 */
bool	is_ambiguous(char *file)
{
	char	*tmp;

	if (!file || file[0] == '\0')
		return (true);
	tmp = file;
	while (*tmp)
	{
		if (*tmp == ' ')
			break ;
		tmp++;
	}
	if (*tmp == ' ')
		return (true);
	return (false);
}

/*
 * Completes execution process with proper signal handling and child waiting.
 * Sets up parent process signal handling for waiting state, waits for all
 * child processes to complete, then restores original signal handlers.
 * Used to properly finish command execution in pipeline.
 *
 * Side effects: Modifies signal handlers, waits for child processes
 */
void	finish_exec(void)
{
	int	status;

	setup_parent_waiting_signals();
	ft_wait_children(&status);
	restore_signals();
}
