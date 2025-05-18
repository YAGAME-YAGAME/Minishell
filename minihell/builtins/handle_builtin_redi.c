/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:43:34 by yagame            #+#    #+#             */
/*   Updated: 2025/05/18 20:29:48 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	open_output(t_redi_list *output)
{
	while (output)
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
	}
	return (1);
}

int	open_input(t_redi_list *input)
{
	int	in_fd;

	while (input)
	{
		if (input->type == INPUT)
		{
			in_fd = ft_open_redi_builtin(input->file, 1);
			if (in_fd == -1)
				return (-1);
			close(in_fd);
		}
		if (input->type == HEREDOC && input->content)
			free(input->content);
		input = input->next;
	}
	return (1);
}

int	open_builtin_redi(t_cmdarg *cmdarg_list)
{
	t_redi_list	*input;
	t_redi_list	*output;

	cmdarg_list->origin_stdin = dup(STDIN_FILENO);
	if (cmdarg_list->origin_stdin == -1)
		return (perror("dup"), g_exit_status = 1, 1);
	cmdarg_list->origin_stdout = dup(STDOUT_FILENO);
	if (cmdarg_list->origin_stdout == -1)
		return (perror("dup"), g_exit_status = 1, 1);
	input = cmdarg_list->input;
	output = cmdarg_list->output;
	if (open_input(input) == -1)
		return (1);
	if (open_output(output) == -1)
		return (1);
	return (0);
}

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
		if (cmdarg_list->input || cmdarg_list->output)
			if (open_builtin_redi(cmdarg_list) == 1)
				return (1);
		if (run_built_in(cmdarg_list, minienv) == 1)
		{
			if (cmdarg_list->output)
				ft_reset_std(cmdarg_list);
			return (1);
		}
	}
	return (0);
}
