/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:57:16 by otzarwal          #+#    #+#             */
/*   Updated: 2025/04/25 21:11:01 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		size_list(t_cmdarg *node)
{
	int i;

	i = 0;
	if(!node)
		return (0);
	while(node)
	{
		i++;
		node = node->next;
	}
	return (i);
}

int 	create_pipe(int *pip)
{
	int p;

	p = pipe(pip);
	if(p == -1)
	{
		write(2, "pipe failure\n", 13);
		return (0);
	}
	return (1);
}

int	execution(t_cmdarg *shell, t_list *env)
{
	t_cmdarg *current_cmd;
	int 	pip_fd[2];
	pid_t	pid;
	int 	status;
	int 	tmp_in;
	
	tmp_in = 0; // Initialize tmp_in to 0 (default stdin)
	current_cmd = shell;
	while(current_cmd)
	{
		if(current_cmd->next)
			if(!create_pipe(pip_fd)) // Create a pipe for the current command
				return (0);
		if((pid = fork()) == -1)
		{
			perror("fork failure\n");
			return (0);
		}
		if(pid == 0)
		{
			if(current_cmd->next)
				close(pip_fd[0]); 
			ft_child(current_cmd, env, tmp_in, pip_fd);
		}
		else
		{
			if(tmp_in != 0)
				close(tmp_in);
			if(current_cmd->next)
			{
				close(pip_fd[1]);
				tmp_in = pip_fd[0];
			}
		}
		current_cmd = current_cmd->next;
	}
	while(wait(&status) > 0)
		; 
	return (1);
}

// printf("cmd -> %s\n", tmp->strags);
