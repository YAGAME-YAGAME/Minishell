/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:55:24 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/07 17:48:13 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_dp(char **cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (0);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (0);
	else if (ft_strcmp(cmd, "export") == 0)
		return (0);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (0);
	else if (ft_strcmp(cmd, "env") == 0)
		return (0);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (0);
	return (-1);
}

int	run_built_in(t_cmdarg *shell, t_list **env)
{
	int	result;

	result = 0;
	if (ft_strcmp(shell->cmd[0], "echo") == 0)
		result = ft_echo(shell->cmd, shell);
	else if (ft_strcmp(shell->cmd[0], "cd") == 0)
		result = ft_cd(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "pwd") == 0)
		result = ft_pwd(env);
	else if (ft_strcmp(shell->cmd[0], "export") == 0)
		result = ft_export(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "unset") == 0)
		result = ft_unset(shell->cmd, env);
	else if (ft_strcmp(shell->cmd[0], "env") == 0)
		result = ft_env(env);
	else if (ft_strcmp(shell->cmd[0], "exit") == 0)
		result = ft_exit(shell->cmd, env);
	g_exit_status = result;
	return (1);
}
