/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:17:54 by codespace         #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Opens a file for builtin command redirection with appropriate flags.
 * Handles file opening for builtin redirections with different modes:
 * flag 0 for output (create/truncate), flag 1 for input (read-only),
 * flag 2 for append. Performs validation and error handling specific
 * to builtin command redirection needs.
 *
 * @param file: Filename to open
 * @param flag: Mode flag (0=output, 1=input, 2=append)
 * @return: File descriptor on success, -1 on failure
 * Side effects: Opens file, prints error messages, sets global exit status
 */
int	ft_open_redi_builtin(char *file, int flag)
{
	int	fd;

	fd = 0;
	if (file[0] == '\0')
		return (ft_redi_error(file, "No such file or directory\n", 1), -1);
	if (is_ambiguous(file) == true)
		return (ft_redi_error(file, "ambiguous redirect\n", 1), -1);
	if (flag == 0)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (flag == 1)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == ENOENT)
			ft_redi_error(file, "No such file or directory\n", 1);
		else if (errno == EISDIR)
			ft_redi_error(file, "Is a directory\n", 1);
		else
			ft_redi_error(file, "Permission denied\n", 1);
	}
	return (fd);
}

/*
 * Restores original stdin and stdout for builtin commands.
 * After builtin command execution with redirections, restores the
 * saved original file descriptors to return stdin/stdout to their
 * previous state. Closes the saved descriptors after restoration.
 *
 * @param shell: Command structure containing saved file descriptors
 * Side effects: Restores file descriptors, closes saved descriptors, prints errors
 */
void	ft_reset_std(t_cmdarg *shell)
{
	if (dup2(shell->origin_stdout, STDOUT_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	if (dup2(shell->origin_stdin, STDIN_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	close(shell->origin_stdout);
	close(shell->origin_stdin);
	shell->origin_stdout = -1;
	shell->origin_stdin = -1;
	return ;
}

/*
 * Prints redirection error messages in minishell format.
 * Formats and displays error messages for redirection failures
 * using the standard minishell error format. Sets the global
 * exit status to indicate the error condition.
 *
 * @param file: Filename that caused the error (can be NULL)
 * @param msg: Error message to display
 * @param err: Error code to set as global exit status
 * Side effects: Writes error message to stderr, sets global exit status
 */
void	ft_redi_error(char *file, char *msg, int err)
{
	write(2, "minishell : ", 11);
	if (file)
	{
		write(2, file, ft_strlen(file));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	g_exit_status = err;
}
