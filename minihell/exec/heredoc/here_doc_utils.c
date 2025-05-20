/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:31:06 by yagame            #+#    #+#             */
/*   Updated: 2025/05/20 16:27:46 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_int_list_heredoc(t_list_heredoc *list)
{
	list->line = NULL;
	list->delimiter = NULL;
	list->fd = -1;
}

void	init_redi_file(t_cmdarg *shell)
{
	t_redi_list	*in;
	t_redi_list	*out;

	in = shell->input;
	out = shell->output;
	while (in)
	{
		if (in->next == NULL)
			in->is_last = true;
		else
			in->is_last = false;
		in->content = NULL;
		in = in->next;
	}
	while (out)
	{
		if (out->next == NULL)
			out->is_last = true;
		else
			out->is_last = false;
		out->tmp_fd = -1;
		out->content = NULL;
		out = out->next;
	}
}

void	ft_read_line(char *delimiter, int *fd_pipe, t_redi_list *heredoc,
		t_list *env)
{
	char	*line;

	line = NULL;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (line == NULL)
		{
			write(1, "\n", 1);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			line = NULL;
			g_exit_status = 0;
			break ;
		}
		if (heredoc->expand)
			ft_expand_var_in_char(&line, env);
		if (heredoc->is_last)
			heredoc->content = my_strjoin(heredoc->content, line);
		free(line);
		line = NULL;
	}
	if (heredoc->is_last)
		write(fd_pipe[1], heredoc->content, ft_strlen(heredoc->content));
	close(fd_pipe[1]);
}
