/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:43:34 by yagame            #+#    #+#             */
/*   Updated: 2025/06/09 18:40:35 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Opens a file for append redirection in builtin command context.
 * Handles the append redirection (>>) for builtin commands by opening
 * the target file in append mode and redirecting stdout if this is
 * the last output redirection in the chain.
 *
 * @param output: Output redirection structure for append operation
 * @return: 1 on success, 0 on failure
 * Side effects: Opens file, may redirect stdout, closes file descriptors
 */
int	open_append(t_redi_list *output)
{
	output->tmp_fd = ft_open_redi_builtin(output->file, 2);
	if (output->tmp_fd == -1)
		return (0);
	if (output->is_last)
	{
		dup2(output->tmp_fd, STDOUT_FILENO);
		close(output->tmp_fd);
	}
	else
		close(output->tmp_fd);
	return (1);
}

/*
 * Processes all output redirections for builtin commands.
 * Iterates through the output redirection list and handles both
 * normal output redirection (>) and append redirection (>>).
 * Only the last redirection in the chain actually affects stdout.
 *
 * @param output: Head of output redirection linked list
 * @return: 1 on success, -1 on failure
 * Side effects: Opens files, may redirect stdout, manages file descriptors
 */
int	open_output(t_redi_list *output)
{
	if (output->type == OUTPUT)
	{
		output->tmp_fd = ft_open_redi_builtin(output->file, 0);
		if (output->tmp_fd == -1)
			return (-1);
		if (output->is_last)
		{
			dup2(output->tmp_fd, STDOUT_FILENO);
			close(output->tmp_fd);
		}
		close(output->tmp_fd);
	}
	if (output->type == APPEND)
		if (!open_append(output))
			return (-1);
	output = output->next;
	return (1);
}

/*
 * Processes all input redirections for builtin commands.
 * Validates input redirections by attempting to open input files
 * and cleans up heredoc content. Does not actually redirect stdin
 * for builtins as most don't read from stdin.
 *
 * @param input: Head of input redirection linked list
 * @return: 1 on success, -1 on failure
 * Side effects: Opens/closes files, frees heredoc content
 */
int	open_input(t_redi_list *input)
{
	int	in_fd;

	if (input->type == INPUT)
	{
		in_fd = ft_open_redi_builtin(input->file, 1);
		if (in_fd == -1)
			return (-1);
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	input = input->next;
	return (1);
}

/*
 * Sets up redirections for builtin command execution.
 * Saves original stdin/stdout file descriptors and processes both
 * input and output redirections for builtin commands. Builtin commands
 * need special redirection handling since they run in the parent process.
 *
 * @param cmdarg_list: Command structure containing redirection information
 * @return: 0 on success, 1 on failure
 * Side effects: Saves original descriptors, processes redirections,
 * sets exit status
 */
int	open_builtin_redi(t_cmdarg *cmdarg_list)
{
	if (cmdarg_list == NULL || cmdarg_list->redirections == NULL)
		return (1);
	cmdarg_list->origin_stdin = dup(STDIN_FILENO);
	if (cmdarg_list->origin_stdin == -1)
		return (perror("dup"), g_exit_status = 1, 1);
	cmdarg_list->origin_stdout = dup(STDOUT_FILENO);
	if (cmdarg_list->origin_stdout == -1)
		return (perror("dup"), g_exit_status = 1, 1);
	if (open_redirections(cmdarg_list->redirections) == -1)
		return (1);
	return (0);
}

/*
 * Checks if command is a builtin and handles its execution with redirections.
 * Determines if the command is a builtin, and if it's the only command
 * in the pipeline, handles its redirections and execution. Builtin commands
 * run in the parent process and need special redirection management.
 *
 * @param cmdarg_list: Command argument structure to check and execute
 * @param minienv: Environment variables for builtin execution
 * @return: 1 if builtin was executed, 0 if not a builtin or multiple commands
 * Side effects: May execute builtin, handle redirections, restore stdout
 */
int	check_builtin(t_cmdarg *cmdarg_list, t_list **minienv)
{
	int	check;

	if (cmdarg_list == NULL)
		return (1);
	if (cmdarg_list->cmd == NULL)
		return (0);
	check = is_builtin(cmdarg_list->cmd[0]);
	if (size_list(cmdarg_list) == 1 && !check)
	{
		if (cmdarg_list->redirections && open_builtin_redi(cmdarg_list) == 1)
		{
			if (cmdarg_list->redirections)
				ft_reset_std(cmdarg_list);
			return (1);
		}
		if (run_built_in(cmdarg_list, minienv) == 1)
		{
			if (cmdarg_list->redirections)
				ft_reset_std(cmdarg_list);
			return (1);
		}
	}
	return (0);
}
