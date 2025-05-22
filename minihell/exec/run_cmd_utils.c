/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:13:00 by codespace         #+#    #+#             */
/*   Updated: 2025/05/21 22:54:09 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_heredoc(t_redi_list *input)
{
	if (dup2(input->heredoc_fd, STDIN_FILENO) == -1)
		ft_cmd_error(NULL, "dup2 failure\n", 1);
	close(input->heredoc_fd);
}

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

void	handle_output(t_redi_list *output)
{
	int	out_fd;

	while (output)
	{
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
}

void	handle_input(t_redi_list *input)
{
	int	in_fd;

	while (input)
	{
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
		if (input->type == HEREDOC && input->is_last)
			handle_heredoc(input);
		input = input->next;
	}
}
