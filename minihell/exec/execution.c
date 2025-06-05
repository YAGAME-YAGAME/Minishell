/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:57:16 by otzarwal          #+#    #+#             */
/*   Updated: 2025/06/05 13:36:25 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Closes heredoc file descriptors for input redirections.
 * Iterates through input redirection list and closes any heredoc
 * file descriptors that are marked as the last (final) redirection
 * to clean up resources after command execution.
 *
 * @param input: Linked list of input redirections to process
 * Side effects: Closes file descriptors marked as last heredoc
 */
void	ft_close_pipe(t_redi_list *input)
{
	while (input)
	{
		if (input->type == HEREDOC && input->is_last)
			close(input->heredoc_fd);
		input = input->next;
	}
}

/*
 * Creates a pipe for inter-process communication.
 * Attempts to create a pipe using the pipe system call and handles
 * errors by setting the global exit status and printing error messages.
 *
 * @param pip: Array to store the read and write file descriptors
 * @return: 1 on success, 0 on failure
 * Side effects: Sets global exit status on error, prints error message
 */
int	create_pipe(int *pip)
{
	int	p;

	p = pipe(pip);
	if (p == -1)
	{
		ft_cmd_error(NULL, "pipe error\n", 1);
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

/*
 * Handles parent process responsibilities in pipeline execution.
 * Manages file descriptor cleanup for the parent process, including
 * closing previous input pipes and setting up the next input for
 * the pipeline. Also cleans up heredoc file descriptors.
 *
 * @param tmp_in: Pointer to temporary input file descriptor
 * @param pip_fd: Current pipe file descriptors
 * @param current_cmd: Current command being processed
 * Side effects: Closes file descriptors, modifies tmp_in value
 */
void	ft_parent(int *tmp_in, int *pip_fd, t_cmdarg *current_cmd)
{
	if (*tmp_in != 0)
		close(*tmp_in);
	if (current_cmd->next)
	{
		close(pip_fd[1]);
		*tmp_in = pip_fd[0];
	}
	ft_close_pipe(current_cmd->input);
}

/*
 * Waits for all child processes to complete and determines final exit status.
 * Collects exit status from all child processes, handling both normal
 * termination and signal termination. Sets the global exit status to
 * the exit status of the last process in the pipeline.
 *
 * @param status: Pointer to integer for storing wait status
 * Side effects: Modifies global exit status, waits for child processes
 */
void	ft_wait_children(int *status)
{
	int	last_status;

	last_status = 0;
	while (wait(status) > 0)
	{
		if (WIFEXITED(*status))
			last_status = WEXITSTATUS(*status);
		else if (WIFSIGNALED(*status))
		{
			if (WTERMSIG(*status) == SIGPIPE)
				continue ;
			last_status = 128 + WTERMSIG(*status);
		}
	}
	g_exit_status = last_status;
}

/*
 * Executes a pipeline of commands with proper process management.
 * Main execution function that creates child processes for each command
 * in the pipeline, sets up pipes between them, and coordinates the
 * execution. Handles process creation, pipe management, and cleanup.
 *
 * @param shell: Linked list of commands to execute in pipeline
 * @param env: Environment variables for command execution
 * @return: 1 on successful setup, 0 on failure
 * Side effects: Creates child processes, sets up pipes,
 * modifies global exit status
 */
int	execution(t_cmdarg *shell, t_list *env)
{
	t_cmdarg	*current_cmd;
	int			pip_fd[2];
	pid_t		pid;
	int			tmp_in;

	tmp_in = 0;
	current_cmd = shell;
	while (current_cmd)
	{
		if (current_cmd->next)
			if (!create_pipe(pip_fd))
				return (0);
		pid = fork();
		if (pid == -1)
			return (g_exit_status = 1, perror("minishell: fork"), 0);
		if (pid == 0)
			ft_child(current_cmd, env, tmp_in, pip_fd);
		else
			ft_parent(&tmp_in, pip_fd, current_cmd);
		current_cmd = current_cmd->next;
	}
	finish_exec();
	return (1);
}
