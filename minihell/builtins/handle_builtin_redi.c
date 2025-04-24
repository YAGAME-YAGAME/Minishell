/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:43:34 by yagame            #+#    #+#             */
/*   Updated: 2025/04/24 10:45:54 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_append(t_redi_list *output)
{
	if (!output)
		return (0);
	output->tmp_fd = open(output->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if(output->tmp_fd == -1)
	{
		write(2, output->file, ft_strlen(output->file));
		write(2, "failure to open out file\n", 25);
		return (0);
	}
	if(!output->is_last)
		close(output->tmp_fd);
	return (1);
}

int	open_output(t_redi_list *output)
 {
	if(!output)
		return (0);
	output->original_fd = dup(STDOUT_FILENO);

	while(output)
	{
		if(output->type == OUTPUT)
		{
			output->tmp_fd = open(output->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(output->tmp_fd == -1)
			{
				write(2, output->file, ft_strlen(output->file));
				write(2, " : failure to open out file\n", 19);
				return (-1);
			}
			if(output->is_last)
			{
				dup2(output->tmp_fd, STDOUT_FILENO);
				close(output->tmp_fd);
			}
			close(output->tmp_fd);
		}
		if(output->type == APPEND)
			if(!open_append(output))
				return (-1);
		output = output->next;
	}
	return (1);
 }


int	open_input(t_redi_list *input)
{
	if(!input)
		return (0);
	int in_fd;

	while(input)
	{
		if(input->type == INPUT)
		{
			in_fd = open(input->file, O_RDONLY);
			if(in_fd == -1)
			{
				write(2, input->file, strlen(input->file));
				write(2, ": No such file or directory\n", 28);
				return (-1);
			}
			close(in_fd);
		}
		if(input->type == HEREDOC && input->content)
			free(input->content);
		input = input->next;
	}
	return (1);
}

int	open_buitlin_redi(t_cmdarg *cmdarg_list)
{
	t_redi_list	*input;
	t_redi_list	*output;

	if (cmdarg_list == NULL)
		return (1);
	input = cmdarg_list->input;
	output = cmdarg_list->output;

	if (open_input(input) == -1)
		return (1);
	if (open_output(output) == -1)
		return (1);
	return (0);
}
void	reset_stdout(t_redi_list *output)
{
	if(!output)
		return ;
	if(dup2(output->original_fd, STDOUT_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	close(output->original_fd);
}

int	check_builtin(t_cmdarg *cmdarg_list, t_list *minienv, char *input)
{
	char **cmd;
	int check;

	if (cmdarg_list == NULL)
		return (1);
	cmd = handel_quote(parsing_split(cmdarg_list->strags, ' '));
	if(cmd == NULL)
		return (1);
	check = is_builtin(cmd[0]);
	free_dp(cmd);
	if(size_list(cmdarg_list) == 1 && !check )
	{
		if (cmdarg_list->input || cmdarg_list->output)
		if (open_buitlin_redi(cmdarg_list) == 1)
		return (1);
		if (run_built_in(cmdarg_list, minienv, input) == 1)
		{
			if (cmdarg_list->output)
				reset_stdout(cmdarg_list->output);
			return (1);
		}
	}
	return (0);
}
