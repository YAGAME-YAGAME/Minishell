/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:13:00 by codespace         #+#    #+#             */
/*   Updated: 2025/06/09 14:41:10 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Handles heredoc input redirection for command execution.
 * Duplicates the heredoc file descriptor to stdin and closes the original
 * file descriptor. Used when a command needs to read from a heredoc
 * that was previously processed and stored in a temporary file.
 *
 * @param input: Input redirection node containing heredoc file descriptor
 * Side effects: Modifies stdin file descriptor, closes heredoc fd, may exit
 * on error
 */
void	handle_heredoc(t_redi_list *input)
{
	if (dup2(input->heredoc_fd, STDIN_FILENO) == -1)
		ft_cmd_error(NULL, "----------- dup2 failure\n", 1);
	close(input->heredoc_fd);
}

/*
 * Handles append output redirection (>>).
 * Opens the target file in append mode and duplicates the file descriptor
 * to stdout if this is the last output redirection. Checks for ambiguous
 * redirections when the filename comes from variable expansion.
 *
 * @param output: Output redirection node for append operation
 * @return: Always returns 1 (success indicator)
 * Side effects: Opens file, modifies stdout if last redirection,
 * may exit on error
 */
int	handel_append(t_redi_list *output)
{
	int	out_fd;

	if (output->variable)
		if (is_ambiguous(output->file) == true)
			ft_cmd_error(output->file, "ambiguous redirect\n", 1);
	out_fd = ft_open_file(output->file, 2);
	if (output->is_last)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			close(out_fd);
			ft_cmd_error(NULL, "dup2 failure\n", 1);
		}
		close(out_fd);
	}
	return (1);
}

/*
 * Processes all output redirections for a command.
 * Iterates through the output redirection list, handling both regular
 * output (>) and append (>>) redirections. Only the last redirection
 * in the list actually redirects stdout, earlier ones just open/close files.
 *
 * @param output: Head of output redirection linked list
 * Side effects: Opens files, may modify stdout, closes file descriptors,
 * may exit on error
 */
void	handle_output(t_redi_list *output)
{
	int	out_fd;

	if (output->type == OUTPUT)
	{
		if (output->variable)
			if (is_ambiguous(output->file) == true)
				ft_cmd_error(output->file, "ambiguous redirect\n", 1);
		out_fd = ft_open_file(output->file, 0);
		if (output->is_last)
		{
			if (dup2(out_fd, STDOUT_FILENO) == -1)
			{
				close(out_fd);
				ft_cmd_error(NULL, "dup2 failure\n", 1);
			}
		}
		close(out_fd);
	}
	if (output->type == APPEND)
		handel_append(output);
	output = output->next;
}

/*
 * Processes all input redirections for a command.
 * Iterates through the input redirection list, handling both regular
 * input (<) and heredoc (<<) redirections. Only the last redirection
 * in the list actually redirects stdin, earlier ones just open/close files.
 *
 * @param input: Head of input redirection linked list
 * Side effects: Opens files, may modify stdin, closes file descriptors,
 * may exit on error
 */
void	handle_input(t_redi_list *input)
{
	int	in_fd;

	if (input->type == INPUT)
	{
		if (input->variable)
			if (is_ambiguous(input->file) == true)
				ft_cmd_error(input->file, "ambiguous redirect\n", 1);
		in_fd = ft_open_file(input->file, 1);
		if (input->is_last)
		{
			if (dup2(in_fd, STDIN_FILENO) == -1)
			{
				close(in_fd);
				ft_cmd_error(NULL, "dup2 failure\n", 1);
			}
		}
		close(in_fd);
	}
	if (input->type == HEREDOC && input->is_last == true)
	{
		handle_heredoc(input);
	}
	input = input->next;
}
