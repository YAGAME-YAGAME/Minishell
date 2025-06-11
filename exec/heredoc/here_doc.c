/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:50:13 by yagame            #+#    #+#             */
/*   Updated: 2025/06/09 14:36:02 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
 * Opens and processes a heredoc in a child process.
 * Creates a child process to read heredoc input until the delimiter
 * is encountered, handling variable expansion and signal management.
 * Writes input to the pipe for the parent process to consume.
 *
 * @param heredoc: Redirection structure containing heredoc information
 * @param fd_pipe: Pipe file descriptors for communication with parent
 * @param env: Environment variables for potential expansion
 * @return: Function exits child process, does not return
 * Side effects: Exits child process, sets up signals, reads from stdin
 */
int	open_here_doc(t_redi_list *heredoc, int *fd_pipe, t_list *env)
{
	char	*delimiter;

	close(fd_pipe[0]);
	delimiter = NULL;
	setup_heredoc_signals();
	delimiter = my_strjoin(heredoc->file, "\n");
	if (!delimiter)
		ft_cmd_error(NULL, "malloc failure\n", 1);
	ft_read_line(delimiter, fd_pipe, heredoc, env);
	free(delimiter);
	exit(0);
}

/*
 * Handles parent process responsibilities for heredoc processing.
 * Manages cleanup of pipe file descriptors, waits for child process
 * completion, and stores the heredoc file descriptor if this is the
 * last heredoc redirection for the command.
 *
 * @param fd_pipe: Pipe file descriptors from heredoc communication
 * @param pid: Process ID of child handling heredoc input
 * @param status: Pointer to store child process exit status
 * @param in: Input redirection structure to update with heredoc fd
 * Side effects: Closes pipes, waits for child, modifies global exit status
 */
void	parent(int *fd_pipe, int pid, int *status, t_redi_list *in)
{
	close(fd_pipe[1]);
	waitpid(pid, status, 0);
	g_exit_status = WEXITSTATUS(*status);
	if (g_exit_status == 0 && in->is_last)
		in->heredoc_fd = fd_pipe[0];
	else
	{
		close(fd_pipe[0]);
		in->heredoc_fd = -1;
	}
}

/*
 * Handles a single heredoc redirection by creating child process.
 * Sets up the pipe communication, forks a child to handle input reading,
 * and manages signal handling for both parent and child processes.
 * Coordinates the complete heredoc processing workflow.
 *
 * @param in: Input redirection structure containing heredoc information
 * @param fd_pipe: Pipe file descriptors for heredoc communication
 * @param env: Environment variables for expansion in heredoc content
 * @return: 1 on success, -1 on failure
 * Side effects: Creates child process, sets up signals, modifies global
 * exit status
 */
int	handel_heredoc(t_redi_list *redi, t_list *env)
{
	int	pid;
	int	status;
	int	fd_pipe[2];

	if (redi == NULL)
		return (1);
	status = 0;
	setup_parent_heredoc_signals();
	if (redi->type == HEREDOC)
	{
		if (pipe(fd_pipe) == -1)
			ft_cmd_error(NULL, "pipe failure", 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), -1);
		if (pid == 0)
			open_here_doc(redi, fd_pipe, env);
		else
		{
			parent(fd_pipe, pid, &status, redi);
			restore_signals();
		}
	}
	return (1);
}

/*
 * Processes all heredoc redirections for all commands in the pipeline.
 * Iterates through each command and handles all heredoc redirections,
 * setting up the necessary file descriptors and managing the input
 * collection process. Initializes redirection file metadata.
 *
 * @param shell: Linked list of commands containing heredoc redirections
 * @param env: Environment variables for heredoc content expansion
 * @return: 1 on success, 0 on failure or interruption
 * Side effects: Modifies redirection structures, processes user input,
 * sets global exit status
 */
int	check_here_doc(t_cmdarg *shell, t_list *env)
{
	t_cmdarg	*tmp;
	t_redi_list	*redi;

	tmp = shell;
	g_exit_status = 0;
	if (!tmp)
		return (1);
	while (tmp)
	{
		init_redi_file(tmp);
		redi = tmp->redirections;
		tmp->origin_stdin = -1;
		tmp->origin_stdout = -1;
		while (redi)
		{
			if (handel_heredoc(redi, env) == -1 || g_exit_status == 1)
				return (0);
			redi = redi->next;
		}
		tmp = tmp->next;
	}
	return (1);
}
