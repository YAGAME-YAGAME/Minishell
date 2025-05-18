/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:50:13 by yagame            #+#    #+#             */
/*   Updated: 2025/05/18 19:09:09 by otzarwal         ###   ########.fr       */
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

int	open_here_doc(t_redi_list *heredoc, t_list *env)
{
	t_list_heredoc	*p;

	p = malloc(sizeof(t_list_heredoc));
	if (!p)
		ft_cmd_error(NULL, "malloc failure\n", 1);
	setup_heredoc_signals();
	ft_int_list_heredoc(p);
	p->fd = open(HEREDOC_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (p->fd == -1)
		ft_cmd_error(NULL, "open failure\n", 1);
	p->delimiter = ft_strjoin(heredoc->file, "\n");
	if (!p->delimiter)
	{
		if (p->line)
			free(p->line);
		ft_cmd_error(NULL, "malloc failure\n", 1);
	}
	ft_read_line(p, heredoc, env);
	close(p->fd);
	ft_free_list_heredoc(p);
	exit(0);
}

static void	ft_parent_proc(int *status, int pid)
{
	waitpid(pid, status, 0);
	g_exit_status = WEXITSTATUS(*status);
	restore_signals();
}

int	handel_heredoc(t_redi_list *in, t_list *env)
{
	int	pid;
	int	status;

	status = 0;
	if (in->type == HEREDOC)
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), -1);
		if (pid == 0)
			open_here_doc(in, env);
		else
			ft_parent_proc(&status, pid);
	}
	return (1);
}

int	check_here_doc(t_cmdarg *shell, t_list *env)
{
	t_cmdarg	*tmp;
	t_redi_list	*in;
	int			ret;

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
			ret = handel_heredoc(in, env);
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
