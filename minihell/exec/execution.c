/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:57:16 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/13 00:19:25 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	size_list(t_cmdarg *node)
{
	int	i;

	i = 0;
	if (!node)
		return (0);
	while (node)
	{
		i++;
		node = node->next;
	}
	return (i);
}

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

void	ft_parent(int *tmp_in, int *pip_fd, t_cmdarg *current_cmd)
{
	if (*tmp_in != 0)
		close(*tmp_in);
	if (current_cmd->next)
	{
		close(pip_fd[1]);
		*tmp_in = pip_fd[0];
	}
}

void ft_wait_children(int *status)
{
    int last_status = 0;
    pid_t last_pid;

    /* Wait for all child processes and use exit status of last command */
    while ((last_pid = wait(status)) > 0)
    {
        if (WIFEXITED(*status))
            last_status = WEXITSTATUS(*status);
        else if (WIFSIGNALED(*status))
        {
            /* Ignore SIGPIPE in pipelines - this is normal when writer's pipe is closed */
            if (WTERMSIG(*status) == SIGPIPE)
                continue;
            last_status = 128 + WTERMSIG(*status);
        }
    }
    g_exit_status = last_status;
}

int	execution(t_cmdarg *shell, t_list *env)
{
	t_cmdarg	*current_cmd;
	int			pip_fd[2];
	pid_t		pid;
	int			status;
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
	return (ft_wait_children(&status), 1);
}
