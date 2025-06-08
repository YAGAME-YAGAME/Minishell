/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:31:06 by yagame            #+#    #+#             */
/*   Updated: 2025/06/08 01:41:10 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_redi_list	*get_last_input(t_redi_list *redi)
{
	t_redi_list	*last;
	t_redi_list	*tmp;

	last = NULL;
	tmp = redi;
	while(tmp)
	{
		tmp->tmp_fd = -1;
		tmp->is_last = false;
		tmp->heredoc_fd = -1;
		tmp->content = NULL;
		if (tmp->type == INPUT || tmp->type == HEREDOC)
			last = tmp;
		
		tmp = tmp->next;
	}
	return (last);
}
static t_redi_list	*get_last_output(t_redi_list *redi)
{
	t_redi_list	*last;

	last = NULL;
	while (redi)
	{
		redi->tmp_fd = -1;
		redi->is_last = false;
		redi->heredoc_fd = -1;
		redi->content = NULL;
		if(redi->type == OUTPUT || redi->type == APPEND)
			last = redi;
		redi = redi->next;
	}
	return (last);
}

/*
 * Initializes redirection file metadata for input and output redirections.
 * Sets the is_last flag for the final redirection in each list and
 * initializes file descriptor values. This metadata is crucial for
 * determining which redirections should actually affect the command's
 * stdin/stdout and which are intermediate operations.
 *
 * @param shell: Command structure containing input/output redirection lists
 * Side effects: Modifies is_last flags and initializes file descriptors
 */
void	init_redi_file(t_cmdarg *shell)
{
	// t_redi_list	*redi;
	t_redi_list	*last_input;
	t_redi_list	*last_output;
	t_cmdarg	*tmp;

	tmp = shell;
	last_input = NULL;
	last_output = NULL;
	if (!tmp)
		return ;
	while (tmp)
	{
		
		last_input = get_last_input(tmp->redirections);
		last_output = get_last_output(tmp->redirections);
		if(last_input)
			last_input->is_last = true;
		if(last_output)
			last_output->is_last = true;
		tmp = tmp->next;
	}
}

/*
 * Handles the end of heredoc input collection.
 * Checks for EOF condition (Ctrl+D) or if the input line matches
 * the heredoc delimiter. Cleans up and returns appropriate status
 * to signal completion of heredoc input collection.
 *
 * @param line: Current input line from user
 * @param delimiter: Heredoc delimiter string to match
 * @return: 1 if heredoc should end, 0 to continue reading
 * Side effects: May free line memory, sets global exit status
 */
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

/*
 * Reads and processes heredoc input until delimiter is encountered.
 * Continuously prompts for input, processes each line (including variable
 * expansion if needed), and accumulates content for the heredoc. Handles
 * the complete input collection process for heredoc redirections.
 *
 * @param delimiter: String that marks the end of heredoc input
 * @param fd_pipe: Pipe file descriptors for writing collected input
 * @param heredoc: Redirection structure containing heredoc metadata
 * @param env: Environment variables for variable expansion
 * Side effects: Reads from stdin, writes to pipe, accumulates content,
 * manages memory
 */
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
