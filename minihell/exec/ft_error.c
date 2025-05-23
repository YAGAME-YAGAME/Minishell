/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:52:05 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/23 06:24:20 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_char_p(char *s)
{
	if (s)
	{
		free(s);
		s = NULL;
	}
}

void	ft_cmd_error(char *cmd_name, char *error, int status)
{
	write(2, "minishell : ", 11);
	if (cmd_name)
	{
		write(2, cmd_name, ft_strlen(cmd_name));
		write(2, ": ", 2);
	}
	write(2, error, ft_strlen(error));
	exit(status);
}

void	ft_error(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int	ft_open_file(char *file, int flag)
{
	int	fd;

	fd = 0;
	if (flag == 0)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (flag == 1)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (errno == ENOENT)
			ft_cmd_error(file, "No such file or directory\n", 1);
		else if (errno == EISDIR)
			ft_redi_error(file, "Is a directory\n", 1);
		else
			ft_redi_error(file, "Permission denied\n", 1);
	}
	return (fd);
}

void	ft_free_and_error(char **cmd_path, char **cmd_name, char **envp)
{
	free(*cmd_path);
	*cmd_path = NULL;
	free_dp(envp);
	*envp = NULL;
	free(*cmd_name);
	*cmd_name = NULL;
	if (errno == EACCES)
		ft_cmd_error(NULL, "Permission denied\n", 126);
	else
		ft_cmd_error(NULL, "execution failure\n", 1);
}
