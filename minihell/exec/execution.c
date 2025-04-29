/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:57:16 by otzarwal          #+#    #+#             */
/*   Updated: 2025/04/29 03:10:03 by yagame           ###   ########.fr       */
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
		ft_cmd_error(NULL, "pipe error\n", 1);
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

void ft_parent(int *tmp_in, int *pip_fd, t_cmdarg *current_cmd)
{
	if(*tmp_in != 0)
		close(*tmp_in);
	if(current_cmd->next)
	{
		close(pip_fd[1]);
		*tmp_in = pip_fd[0];
	}
}

void ft_wait_children(int *status)
{
	while(wait(status) > 0)
	{
		if (WIFEXITED(*status))
			g_exit_status = WEXITSTATUS(*status);
		else if (WIFSIGNALED(*status))
			g_exit_status = 128 + WTERMSIG(*status);
	}
}

int	execution(t_cmdarg *shell, t_list *env)
{
	t_cmdarg *current_cmd;
	int 	pip_fd[2];
	pid_t	pid;
	int 	status;
	int 	tmp_in;
	
	tmp_in = 0; 
	current_cmd = shell;
	while(current_cmd)
	{
		
		if(current_cmd->next)
			if(!create_pipe(pip_fd))
				return (0);
		if((pid = fork()) == -1)
		{
			perror("minishell: fork");
			return (g_exit_status = 1, 0);
		}
		if(pid == 0)
			ft_child(current_cmd, env, tmp_in, pip_fd);
		else
			ft_parent(&tmp_in, pip_fd, current_cmd);
		current_cmd = current_cmd->next;
	}
	ft_wait_children(&status);
	return (1);
}

// printf("cmd -> %s\n", tmp->strags);
