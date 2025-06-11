/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:52:05 by otzarwal          #+#    #+#             */
/*   Updated: 2025/06/09 18:34:27 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Safely frees a character pointer and sets it to NULL.
 * Provides null-safe string deallocation by checking for null pointer
 * before freeing. Sets pointer to NULL after freeing to prevent
 * double-free errors and dangling pointer issues.
 *
 * @param s: Character pointer to free
 * Side effects: Frees memory if pointer is not NULL, sets pointer to NULL
 */
void	ft_free_char_p(char *s)
{
	if (s)
	{
		free(s);
		s = NULL;
	}
}

/*
 * Prints error message and exits with specified status code.
 * Formats and displays shell error messages with consistent format:
 * "minishell : [command]: [error]". Terminates the current process
 * with the provided exit status after printing the error.
 *
 * @param cmd_name: Command name to include in error message (can be NULL)
 * @param error: Error message string to display
 * @param status: Exit status code for process termination
 * Side effects: Writes error to stderr, terminates process
 */
void	ft_cmd_error(char *cmd_name, char *error, int status)
{
	write(2, "minishell : ", 11);
	if (cmd_name)
	{
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": ", 2);
	}
	write(2, error, ft_strlen(error));
	exit(status);
}

/*
 * Opens files with specified flags for redirection operations.
 * Handles different file opening modes: truncate (flag 0), read-only (flag 1),
 * and append (flag 2). Reports appropriate error messages for common
 * failure cases like file not found, directory access, and permission issues.
 *
 * @param file: Filename to open
 * @param flag: Opening mode (0=truncate, 1=read, 2=append)
 * @return: File descriptor on success, -1 on failure (with error reported)
 * Side effects: Opens file, may write error messages, sets g_exit_status
 * on error
 */
int	ft_open_file(char *file, int flag)
{
	int	fd;

	fd = 0;
	if (flag == 0)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (flag == 1)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == ENOENT)
			ft_cmd_error(file, "No such file or directory\n", 1);
		else if (errno == EISDIR)
			ft_cmd_error(file, "Is a directory\n", 1);
		else
			ft_cmd_error(file, "Permission denied\n", 1);
	}
	return (fd);
}

/*
 * Handles cleanup and error reporting for execve failures.
 * Frees allocated memory for command path, name, and environment array
 * before reporting execution errors. Determines appropriate error message
 * and exit code based on errno (permission denied vs general failure).
 *
 * @param cmd_path: Pointer to command path string to free
 * @param cmd_name: Pointer to command name string to free
 * @param envp: Pointer to environment array to free
 * Side effects: Frees memory, reports error, terminates process
 */
void	ft_free_and_error(char **cmd_path, char **cmd_name, char **envp)
{
	free(*cmd_path);
	*cmd_path = NULL;
	free_dp(envp);
	*envp = NULL;
	free(*cmd_name);
	*cmd_name = NULL;
	if (errno == EACCES)
		ft_cmd_error(NULL, "Permission denied\n", 126);
	else
		ft_cmd_error(NULL, "execution failure\n", 1);
}
