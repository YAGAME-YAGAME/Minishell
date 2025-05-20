/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:17:54 by codespace         #+#    #+#             */
/*   Updated: 2025/05/18 19:22:55 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_open_redi_builtin(char *file, int flag)
{
	int	fd;

	fd = 0;
	if (file[0] == '\0')
		return (ft_redi_error(file, "No such file or directory\n", 1), -1);
	if (is_ambiguous(file) == true)
		return (ft_redi_error(file, "ambiguous redirect\n", 1), -1);
	if (flag == 0)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (flag == 1)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == ENOENT)
			ft_redi_error(file, "No such file or directory\n", 1);
		else if (errno == EISDIR)
			ft_redi_error(file, "Is a directory\n", 1);
		else
			ft_redi_error(file, "Permission denied\n", 1);
	}
	return (fd);
}

void	ft_reset_std(t_cmdarg *shell)
{
	if (dup2(shell->origin_stdout, STDOUT_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	if (dup2(shell->origin_stdin, STDIN_FILENO) < 0)
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

void	ft_redi_error(char *file, char *msg, int err)
{
	write(2, "minishell : ", 11);
	if (file)
	{
		write(2, file, ft_strlen(file));
		write(2, ": ", 2);
	}
	write(2, msg, ft_strlen(msg));
	g_exit_status = err;
}
