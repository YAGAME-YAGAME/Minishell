/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:55:24 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Frees a null-terminated array of strings (double pointer).
 * Iterates through the array, freeing each individual string before
 * freeing the array pointer itself. Used for cleaning up command
 * argument arrays and other string arrays.
 *
 * @param cmd: Null-terminated array of strings to free
 * Side effects: Frees all allocated memory for strings and array
 */
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

/*
 * Determines if a command name corresponds to a shell builtin.
 * Checks the command string against all supported builtin commands
 * (echo, cd, pwd, export, unset, env, exit). Used to decide whether
 * to execute a builtin function or search for external programs.
 *
 * @param cmd: Command name string to check
 * @return: 0 if command is a builtin, -1 if not a builtin
 */
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

/*
 * Executes the appropriate builtin command based on command name.
 * Dispatches to the correct builtin function implementation after
 * identifying the command type. Sets the global exit status based
 * on the builtin command's return value.
 *
 * @param shell: Command structure containing command and arguments
 * @param env: Pointer to environment variables linked list
 * @return: Always returns 1 (builtin was executed)
 * Side effects: Executes builtin command, updates g_exit_status
 */
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
