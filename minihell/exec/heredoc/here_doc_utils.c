/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:31:06 by yagame            #+#    #+#             */
/*   Updated: 2025/05/06 19:45:59 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	init_redi_file(t_cmdarg *shell)
{
	t_redi_list *in;
	t_redi_list *out;

	in = shell->input;
	out = shell->output;
	while(in)
	{
		if (in->next == NULL)
			in->is_last = true;
		else
			in->is_last = false;
		in->content = NULL;

		in = in->next;
	}
	while(out)
	{
		if(out->next == NULL)
			out->is_last = true;
		else
			out->is_last = false;
		out->tmp_fd = -1;
		out->content = NULL;
		out = out->next;
	}
}

void ft_read_line(int fd, char **line, char *delimiter, t_redi_list *heredoc, t_list *env)
{
    while(1)
	{
		write(1, "here_doc >> ", 12);
		*line = get_next_line(0);
		if (*line == NULL)
		{
			write(1, "\n", 1);
			if(heredoc->is_last)
				write(fd, *line, ft_strlen(*line));
			break;
		}
		if(ft_strncmp(*line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(*line);
			*line = NULL;
			break;
		}
		if (heredoc->expand)
			ft_expand_var_in_char(line, env);
		if(heredoc->is_last)
			write(fd, *line, ft_strlen(*line));
		free(*line);
		*line = NULL;
	}
}
