/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:57:16 by otzarwal          #+#    #+#             */
/*   Updated: 2025/04/15 17:48:30 by yagame           ###   ########.fr       */
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
int		handel_built_in(t_cmdarg *shell, t_list *env)
{
	(void)shell;
	(void)env;
	printf("wait to handel built_in command here\n");
	// t_cmdarg *tmp;
	// char 	 **cmd;
	// char	 *cmd_path;
	
	
	
	// tmp = shell;
	// cmd = parsing_split(tmp->strags, ' ');
	// if(is_built(cmd[0]))
	// {
	// 	cmd_path = check_exec(cmd[0], env);
	// 	if(!cmd_path)
	// 		ft_error("this command has not path\n");
	// 	exec_built_in(cmd_path, tmp);
	// }
	return (0);
}
void 	create_pipe(int *pip)
{
		int p;

		p = pipe(pip);
		if(p == -1)
			ft_error("pipe failure\n");
}

void	execution(t_cmdarg *shell, t_list *env)
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
			create_pipe(pip_fd); // Create a pipe for the current command
		if((pid = fork()) == -1)
			ft_error("fork failure\n");
		if(pid == 0)
		{
			// Child process
			if(current_cmd->next)
				close(pip_fd[0]); // Close unused read end in child
			ft_child(current_cmd, env, tmp_in, pip_fd);
		}
		else
		{
			// Parent process
			if(tmp_in != 0)
				close(tmp_in); // Close previous input in parent
			if(current_cmd->next)
			{
				close(pip_fd[1]); // Close unused write end in parent
				tmp_in = pip_fd[0]; // Save read end for the next command
			}
		}
		current_cmd = current_cmd->next;
	}
	while(wait(&status) > 0)
		; // Wait for all child processes to finish
}

// printf("cmd -> %s\n", tmp->strags);
