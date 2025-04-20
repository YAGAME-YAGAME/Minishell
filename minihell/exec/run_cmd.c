/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:11 by otzarwal          #+#    #+#             */
/*   Updated: 2025/04/19 17:27:23 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int	handle_execution(t_cmdarg *current_cmd, t_list *env)
{
	char **cmd;
	char *cmd_path;

	cmd = parsing_split(current_cmd->strags, ' ');
	cmd_path = check_exec(cmd[0], env);
	if(!cmd_path)
	{
		write(2, "command not found\n", 18);
		return (0);
	}
	char **envp = NULL;
	envp = get_env(env);
	if(execve(cmd_path, cmd, envp) == -1)
	{
		write(2, "execve failure\n", 15);
		free(cmd_path);
		free(envp);
		return (0);
	}
	return (1);
}

int	handle_heredoc(t_redi_list *input)
{
	int	heredoc_fd[2];

	if(pipe(heredoc_fd) == -1)
	{
		perror("pipe failure\n");
		return (0);
	}
	write(heredoc_fd[1], input->content, ft_strlen(input->content));
	close(heredoc_fd[1]);
	dup2(heredoc_fd[0], STDIN_FILENO);
	close(heredoc_fd[0]);
	free(input->content);
	return (1);
}

int handel_append(t_redi_list *output)
{
	int out_fd;

	out_fd = open(output->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if(out_fd == -1)
	{
		write(2, "output file not found\n", 22);
		return (0);
		
	}
	if(output->is_last)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	else
		close(out_fd);
	return (1);
}

int	handle_output(t_redi_list *output)
 {
	int out_fd;

	while(output)
	{
		if(output->type == OUTPUT)
		{
			out_fd = open(output->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if(out_fd == -1)
			{
				write(2, output->file, ft_strlen(output->file));
				write(2, " : failure to open\n", 19);
				return (0);
			}
			
			if(output->is_last)
			{
				dup2(out_fd, STDOUT_FILENO);
				close(out_fd);
			}
			else
				close(out_fd);
		}
		if(output->type == APPEND)
			if(!handel_append(output))
				return (0);
		output = output->next;
	}
	return (1);
 }
 
int handle_input(t_redi_list *input)
{
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
				return (0);
			}
			if(input->is_last)
			{
				// printf("is last redirection : %s\n", input->file);
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			else
				close(in_fd);
		}
		if(input->type == HEREDOC && input->content)
			if(!handle_heredoc(input))
				return (0);
		input = input->next;
	}
	return (1);
}
// =====================/ end handle input redirection /========================//

int	 ft_child(t_cmdarg *current_cmd, t_list *env, int tmp_in, int *p_fd)
{
	int fd_in;
	int fd_out;
	
	if(tmp_in != 0)
	{
		dup2(tmp_in, STDIN_FILENO);
		close(tmp_in);
	}
	if(current_cmd->next)
	{
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
	}
	fd_in = handle_input(current_cmd->input);
	if(!fd_in)
		return (0);
	fd_out = handle_output(current_cmd->output);
	if(!fd_out)
		return (0);
	if(!handle_execution(current_cmd, env))
		return (0);
	return (1);
}
