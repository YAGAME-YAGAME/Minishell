/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:11 by otzarwal          #+#    #+#             */
/*   Updated: 2025/06/05 04:26:34 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Checks if a given path points to a directory.
 * Uses stat system call to determine if the path exists and represents
 * a directory. Only checks paths that contain forward slashes and are
 * not empty or null.
 *
 * @param path: File path string to check
 * @return: true if path is a directory, false otherwise
 */
bool	ft_isdirectory(char *path)
{
	struct stat	sb;

	if (path == NULL || *path == '\0' || ft_strchr(path, '/') == NULL)
		return (false);
	if (stat(path, &sb) == -1)
		return (false);
	return (S_ISDIR(sb.st_mode));
}

void	ft_free_isdir(char **cmd_path, char **cmd_name, t_cmdarg *current_cmd)
{
	free(*cmd_path);
	free(*cmd_name);
	ft_cmd_error(current_cmd->cmd[0], "is a directory\n", 126);
}

/*
 * Handles the execution of external commands.
 * Manages the complete process of executing non-builtin commands including
 * path resolution, directory checking, environment preparation, and final
 * execution via execve. Handles various error conditions and cleanup.
 *
 * @param current_cmd: Command structure containing command and arguments
 * @param env: Environment variables list
 * Side effects: May exit process, allocates/frees memory, executes command
 */
void	handle_execution(t_cmdarg *current_cmd, t_list *env)
{
	char	*cmd_path;
	char	**envp;
	char	*cmd_name;

	envp = NULL;
	cmd_name = NULL;
	if (current_cmd == NULL || current_cmd->cmd[0] == NULL)
		exit(0);
	if (current_cmd->cmd[0])
		cmd_name = ft_strdup(current_cmd->cmd[0]);
	cmd_path = check_exec(current_cmd->cmd[0], env);
	if (cmd_path == NULL)
	{
		free(cmd_name);
		ft_cmd_error(current_cmd->cmd[0], "command not found\n", 127);
	}
	if (ft_isdirectory(cmd_path))
		ft_free_isdir(&cmd_path, &cmd_name, current_cmd);
	envp = get_env(env);
	if (envp == NULL)
		handle_exec_error(cmd_path, cmd_name);
	if (execve(cmd_path, current_cmd->cmd, envp) == -1)
		ft_free_and_error(&cmd_path, &cmd_name, envp);
}

/*
 * Executes builtin commands in child processes.
 * Checks if the current command is a builtin and executes it if so.
 * Handles the exit status and terminates the child process after
 * builtin execution with the appropriate exit code.
 *
 * @param current_cmd: Command structure to check and execute
 * @param env: Pointer to environment variables list
 * Side effects: May exit process with global exit status
 */
void	ft_is_builtin(t_cmdarg *current_cmd, t_list **env)
{
	char	**cmd;

	if (!current_cmd || !current_cmd->cmd)
		return ;
	cmd = current_cmd->cmd;
	if (cmd && cmd[0] && is_builtin(cmd[0]) == 0)
	{
		if (run_built_in(current_cmd, env))
		{
			exit(g_exit_status);
		}
	}
}

/*
 * Main function for child process execution in pipeline.
 * Sets up the child process environment including signal handling,
 * file descriptor duplication for pipes, redirection handling,
 * and final command execution (either builtin or external).
 *
 * @param current_cmd: Command structure to execute
 * @param env: Environment variables list
 * @param tmp_in: Input file descriptor from previous command in pipeline
 * @param p_fd: Pipe file descriptors for output to next command
 * Side effects: Modifies file descriptors, handles redirections,
 * executes command
 */
void	ft_child(t_cmdarg *current_cmd, t_list *env, int tmp_in, int *p_fd)
{
	setup_child_signals();
	if (tmp_in != 0 && dup2(tmp_in, STDIN_FILENO) == -1)
		ft_cmd_error(NULL, "dup2 failure", 1);
	if (current_cmd->next && dup2(p_fd[1], STDOUT_FILENO) == -1)
		ft_cmd_error(NULL, "dup2 failure", 1);
	if (tmp_in != 0)
		close(tmp_in);
	if (current_cmd->next)
	{
		close(p_fd[1]);
		close(p_fd[0]);
	}
	handle_input(current_cmd->input);
	handle_output(current_cmd->output);
	ft_is_builtin(current_cmd, &env);
	handle_execution(current_cmd, env);
}
