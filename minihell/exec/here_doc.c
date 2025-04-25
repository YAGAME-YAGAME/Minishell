/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:50:13 by yagame            #+#    #+#             */
/*   Updated: 2025/04/25 21:06:23 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void	print_list(t_redi_list *in, t_redi_list *out);

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

void	open_here_doc(t_redi_list *heredoc, t_list *env)
{
	char *line;
	char *content;
	char *delimiter;

	content = malloc(1 * sizeof(char));
	content[0] = '\0';
	delimiter = ft_strjoin(heredoc->file, "\n");
	while(1)
	{
		write(1 ,"here_doc >> ", 12);
		line = get_next_line(0);
		if (heredoc->expand)
			ft_ExpandVarInChar(&line, env);
		if(ft_strncmp(line , delimiter, ft_strlen(delimiter)) == 0)
			break;
		if(heredoc->is_last)
			content = my_strjoin(content, line);
		free(line);
	}
	if(heredoc->is_last)
		heredoc->content = content;
	free(line);
}

void	check_here_doc(t_cmdarg *shell, t_list *env)
{
	t_cmdarg *tmp;
	t_redi_list *in;

	tmp = shell;

	while(tmp)
	{
		init_redi_file(tmp);
		in = tmp->input;
		tmp->origin_stdin = -1;
		tmp->origin_stdout = -1;
		while (in)
		{
			if(in->type == HEREDOC)
				open_here_doc(in, env);
			in = in->next;
		}
		tmp = tmp->next;
	}
}

void	print_list(t_redi_list *in, t_redi_list *out)
{
	static int cmd = 1;
	printf("\n");
	printf("cmd [ %d ]\n", cmd);
	while(in)
	{
		printf("type : %s\nfile : %s\ncontent : %s\nis_last : %d\n",ft_printtype(in->type), in->file, in->content, in->is_last);
		in = in->next;
	}
	printf("\n");
	while(out)
	{
		printf("type : %s\nfile : %s\ncontent : %s\nis_last : %d\n",ft_printtype(out->type), out->file, out->content, out->is_last);
		out = out->next;
	}
	cmd++;
}
