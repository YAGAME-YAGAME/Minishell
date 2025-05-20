/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:50:13 by yagame            #+#    #+#             */
/*   Updated: 2025/05/20 16:17:25 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free_list_heredoc(t_list_heredoc *list)
{
	if (list->line)
		free(list->line);
	if (list->delimiter)
		free(list->delimiter);
	if (list->fd != -1)
		close(list->fd);
	free(list);
}

int	open_here_doc(t_redi_list *heredoc, int *fd_pipe, t_list *env)
{
	char	*delimiter;

	close(fd_pipe[0]);
	delimiter = NULL;
	setup_heredoc_signals();
	delimiter = my_strjoin(heredoc->file, "\n");
	if (!delimiter)
		ft_cmd_error(NULL, "malloc failure\n", 1);
	ft_read_line(delimiter, fd_pipe, heredoc, env);
	exit(0);
}

int	handel_heredoc(t_redi_list *in, int *fd_pipe, t_list *env)
{
	int	pid;
	int	status;
	int	rd;

	if (pipe(fd_pipe) == -1)
		ft_cmd_error(NULL, "pipe failure", 1);
	status = 0;
	if (in->type == HEREDOC)
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), -1);
		if (pid == 0)
			open_here_doc(in, fd_pipe, env);
		else
		{
			close(fd_pipe[1]);
			waitpid(pid, &status, 0);
			g_exit_status = WEXITSTATUS(status);
			if (in->is_last && g_exit_status == 0)
			{
				if (in->content)
					free(in->content);
				in->content = malloc(10000);
				rd = read(fd_pipe[0], in->content, 9999);
				in->content[rd] = '\0';
			}
			else
				free(in->content);
			close(fd_pipe[0]);
			restore_signals();
		}
	}
	return (1);
}

int	check_here_doc(t_cmdarg *shell, t_list *env)
{
	t_cmdarg	*tmp;
	t_redi_list	*in;
	int			ret;
	int			fd_pipe[2];

	tmp = shell;
	ret = 0;
	g_exit_status = 0;
	while (tmp)
	{
		init_redi_file(tmp);
		in = tmp->input;
		tmp->origin_stdin = -1;
		tmp->origin_stdout = -1;
		while (in)
		{
			ret = handel_heredoc(in, fd_pipe, env);
			if (ret == -1 || g_exit_status == 1)
			{
				return (0);
			}
			in = in->next;
		}
		tmp = tmp->next;
	}
	return (1);
}
