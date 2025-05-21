/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:31:06 by yagame            #+#    #+#             */
/*   Updated: 2025/05/22 00:05:47 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

int	handle_heredoc_break(char *line, char *delimiter)
{
	if (line == NULL)
	{
		write(1, "\n", 1);
		return (1);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
	{
		free(line);
		line = NULL;
		g_exit_status = 0;
		return (1);
	}
	return (0);
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
		if (handle_heredoc_break(line, delimiter))
			break ;
		if (heredoc->expand)
			ft_expand_var_in_char(&line, env);
		if (heredoc->is_last)
			heredoc->content = my_strjoin(heredoc->content, line);
		free(line);
		line = NULL;
	}
	if (heredoc->is_last)
		write(fd_pipe[1], heredoc->content, ft_strlen(heredoc->content));
	free(heredoc->content);
	heredoc->content = NULL;
	close(fd_pipe[1]);
}
