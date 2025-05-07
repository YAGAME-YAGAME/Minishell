/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:31:06 by yagame            #+#    #+#             */
/*   Updated: 2025/05/07 23:27:08 by codespace        ###   ########.fr       */
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

void	ft_read_line(t_list_heredoc *p, t_redi_list *heredoc, t_list *env)
{
	while (1)
	{
		write(1, "here_doc >> ", 12);
		p->line = get_next_line(0);
		if (p->line == NULL)
		{
			write(1, "\n", 1);
			if (heredoc->is_last)
				write(p->fd, p->line, ft_strlen(p->line));
			break ;
		}
		if (ft_strncmp(p->line, p->delimiter, ft_strlen(p->delimiter)) == 0)
		{
			free(p->line);
			p->line = NULL;
			break ;
		}
		if (heredoc->expand)
			ft_expand_var_in_char(&p->line, env);
		if (heredoc->is_last)
			write(p->fd, p->line, ft_strlen(p->line));
		free(p->line);
		p->line = NULL;
	}
}
