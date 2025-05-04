/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:50:13 by yagame            #+#    #+#             */
/*   Updated: 2025/05/04 00:01:03 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../minishell.h"

int		open_here_doc(t_redi_list *heredoc, t_list *env)
{
	char *line;
	char *delimiter;
	int fd;

	line = NULL;
	signal(SIGINT, handle_heredoc_sigint);
	fd = open(HEREDOC_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ft_cmd_error(NULL, "open failure\n", 1);
	delimiter = ft_strjoin(heredoc->file, "\n");
	if(!delimiter)
	{
		if (line)
			free(line);
		ft_cmd_error(NULL, "malloc failure\n", 1);
	}
	ft_read_line(fd, &line, delimiter, heredoc, env);
	close(fd);
	free(delimiter);
	exit(0);
}

static void ft_parent_proc(int *status, int pid)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
	{
		if (WEXITSTATUS(*status) != 0)
			g_exit_status = WEXITSTATUS(*status);
	}
	else if (WIFSIGNALED(*status))
		g_exit_status = 128 + WTERMSIG(*status);
	restore_signals();
}

int handel_heredoc(t_redi_list *in, t_list *env)
{
	int pid;
	int status;

	status = 0;
	if(in->type == HEREDOC)
	{
		if((pid = fork()) == -1)
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
	t_cmdarg *tmp;
	t_redi_list *in;
	int ret;

	tmp = shell;
	ret = 0;
	while(tmp)
	{
		init_redi_file(tmp);
		in = tmp->input;
		tmp->origin_stdin = -1;
		tmp->origin_stdout = -1;
		while (in)
		{
			ret = handel_heredoc(in, env);
			if (ret == -1)
				return (0);
			in = in->next;
		}
		tmp = tmp->next;
	}
	return (1);
}
