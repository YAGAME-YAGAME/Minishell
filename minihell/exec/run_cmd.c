/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:11 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/06 19:58:53 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"




void	handle_execution(t_cmdarg *current_cmd, t_list *env)
{
	char *cmd_path;
	char **envp = NULL;
	char *cmd_name = NULL;  // Store command name to avoid use-after-free

	if(current_cmd == NULL || current_cmd->cmd[0] == NULL)
		exit(0);

	// Save command name before we potentially free anything
	if (current_cmd->cmd[0])
		cmd_name = ft_strdup(current_cmd->cmd[0]);

	cmd_path = check_exec(current_cmd->cmd[0], env);
	if(cmd_path == NULL)
	{
		// Use our saved command name instead of accessing potentially freed memory
		ft_cmd_error(cmd_name, "command not found\n", 127);
		// ft_cmd_error exits the process, so this will never return
	}

	envp = get_env(env);
	if (envp == NULL)
	{
		free(cmd_path);
		free(cmd_name);
		ft_cmd_error(NULL, "malloc failure\n", 1);
	}

	if(execve(cmd_path, current_cmd->cmd, envp) == -1)
	{
		free(cmd_path);
		free_dp(envp);
		free(cmd_name);
		if (errno == EACCES)
            ft_cmd_error(cmd_name, "Permission denied\n", 126);
        else
			ft_cmd_error(cmd_name, "execution failure\n", 1);
	}

	// This code won't be reached if execve succeeds
	free(cmd_path);
	free(cmd_name);
	free_dp(envp);
}

void	handle_heredoc(t_redi_list *input)
{
	int fd;
	(void)input;
	fd = ft_open_file(HEREDOC_FILE, 1);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		ft_cmd_error(NULL, "dup2 failure\n", 1);
	}
	close(fd);
}

int handel_append(t_redi_list *output)
{
	int out_fd;

	if(output->variable)
		if(is_ambiguous(output->file) == true)
				ft_cmd_error(output->file, "ambiguous redirect\n", 1);
	out_fd = ft_open_file(output->file, 2);
	if(output->is_last)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			close(out_fd);
			ft_cmd_error(NULL, "dup2 failure\n", 1);
		}
		close(out_fd);
	}
	return (1);
}

void	handle_output(t_redi_list *output)
 {
	int out_fd;

	while(output)
	{
		if(output->type == OUTPUT)
		{
			if(output->variable)
				if(is_ambiguous(output->file) == true)
					ft_cmd_error(output->file, "ambiguous redirect\n", 1);
			out_fd = ft_open_file(output->file, 0);
			if(output->is_last)
			{
				if(dup2(out_fd, STDOUT_FILENO) == -1)
				{
					close(out_fd);
					ft_cmd_error(NULL, "dup2 failure\n", 1);
				}
			}
			close(out_fd);
		}
		if(output->type == APPEND)
			handel_append(output);
		output = output->next;
	}
}


void         handle_input(t_redi_list *input)
{
	int in_fd;

	while(input)
	{
		if(input->type == INPUT)
		{
			if(input->variable)
			{
				if(is_ambiguous(input->file) == true)
					ft_cmd_error(input->file, "ambiguous redirect\n", 1);
			}
			in_fd = ft_open_file(input->file, 1);
			if(input->is_last)
			{
				if(dup2(in_fd, STDIN_FILENO) == -1)
				{
					close(in_fd);
					ft_cmd_error(NULL, "dup2 failure\n", 1);
				}
			}
			close(in_fd);
		}
		if(input->type == HEREDOC && input->is_last)
			handle_heredoc(input);
		input = input->next;
	}
}
// =====================/ end handle input redirection /========================//
void ft_is_builtin(t_cmdarg *current_cmd, t_list **env)
{
	char **cmd;

	if (!current_cmd || !current_cmd->cmd)
		return;

	cmd = current_cmd->cmd;
	// Remove the check for strags as it might be uninitialized
	if (cmd && cmd[0] && is_builtin(cmd[0]) == 0)
	{
		if (run_built_in(current_cmd, env, NULL))
		{
			exit(g_exit_status);
		}
	}
}

void     ft_child(t_cmdarg *current_cmd, t_list *env, int tmp_in, int *p_fd)
{
	if(tmp_in != 0 && dup2(tmp_in, STDIN_FILENO) == -1)
		ft_cmd_error(NULL, "dup2 failure", 1);
	if(current_cmd->next && dup2(p_fd[1], STDOUT_FILENO) == -1)
		ft_cmd_error(NULL, "dup2 failure", 1);

	if(tmp_in != 0)
		close(tmp_in);
	if(current_cmd->next)
	{
		close(p_fd[1]);
		close(p_fd[0]);
	}
	// fprintf(stderr,"%s\n", "hello");

	handle_input(current_cmd->input);
	handle_output(current_cmd->output);
	ft_is_builtin(current_cmd, &env);
	handle_execution(current_cmd, env);
}
