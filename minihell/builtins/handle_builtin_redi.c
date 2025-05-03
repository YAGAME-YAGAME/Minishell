/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:43:34 by yagame            #+#    #+#             */
/*   Updated: 2025/05/04 00:10:34 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_append(t_redi_list *output)
{
	output->tmp_fd = open(output->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if(output->tmp_fd == -1)
	{
		write(2, output->file, ft_strlen(output->file));
		write(2, "failure to open out file\n", 25);
		return (0);
	}
	if(output->is_last)
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
	while(output)
	{
		if(output->type == OUTPUT)
		{
			output->tmp_fd = open(output->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(output->tmp_fd == -1)
			{
				write(2, output->file, ft_strlen(output->file));
				write(2, " : no such file or directory\n", 19);
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
	int	in_fd;


	while(input)
	{
		if(input->type == INPUT)
		{
			in_fd = open(input->file, O_RDONLY);
			if(in_fd == -1)
			{
				perror(input->file);
				write(2, " : failure to open in file\n", 27);
				return (g_exit_status = 1,  -1);
			}
			close(in_fd);
		}
		if(input->type == HEREDOC && input->content)
			free(input->content);
		input = input->next;
	}
	return (1);
}

int open_buitlin_redi(t_cmdarg *cmdarg_list)
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

void	ft_reset_std(t_cmdarg *shell)
{
	if(dup2(shell->origin_stdout, STDOUT_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	if(dup2(shell->origin_stdin, STDIN_FILENO) < 0)
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

int	check_builtin(t_cmdarg *cmdarg_list, t_list **minienv, char *input)
{
	int		check;

	if (cmdarg_list == NULL)
		return (1);
	if(cmdarg_list->cmd == NULL) 
		return (0);

	check = is_builtin(cmdarg_list->cmd[0]);

	if(size_list(cmdarg_list) == 1 && !check)
	{
		if (cmdarg_list->input || cmdarg_list->output)
			if (open_buitlin_redi(cmdarg_list) == 1)
				return (1);
		if (run_built_in(cmdarg_list, minienv, input) == 1)
		{
			if (cmdarg_list->output)
				ft_reset_std(cmdarg_list);
			return (1);
		}
	}
	return (0);
}
