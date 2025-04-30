/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 00:50:13 by yagame            #+#    #+#             */
/*   Updated: 2025/04/30 23:44:12 by yagame           ###   ########.fr       */
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

int		open_here_doc(t_redi_list *heredoc, t_list *env)
{
	char *line;
	char *content;
	char *delimiter;
	int fd;

	fd = open(HEREDOC_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd == -1)
	{
		perror("open");
		return (0);
	}
	
	content = malloc(1 * sizeof(char));
	if(!content)
	{
		perror("malloc failure\n");
		return (0);
	}
	content[0] = '\0';
	delimiter = ft_strjoin(heredoc->file, "\n");
	
	while(1)
	{
		write(1, "here_doc >> ", 12);
		line = get_next_line(0);

		// Handle Ctrl+D or Ctrl+C (NULL return from get_next_line)
		if (line == NULL)
		{
			// Check if we were interrupted by Ctrl+C
			if (g_exit_status == 130)
			{
				free(content);
				free(delimiter);
				exit(130) ;// Signal error to caller
			}
			
			// Normal EOF (Ctrl+D)
			write(1, "\n", 1);
			if(heredoc->is_last)
				write(fd, content, ft_strlen(content));	
			break;
		}
		
		// Check for delimiter
		if(ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		
		// Process line normally
		if (heredoc->expand)
			ft_ExpandVarInChar(&line, env);
		if(heredoc->is_last)
			content = my_strjoin(content, line);
		free(line);
	}
	
	// Save content if not already saved
	if(heredoc->is_last && heredoc->content == NULL)
		write(fd, content, ft_strlen(content));
	close(fd);
	free(content);
	free(delimiter);
	return (1);
}

int	check_here_doc(t_cmdarg *shell, t_list *env)
{
	t_cmdarg *tmp;
	t_redi_list *in;
	int pid;
	int status;

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
			{
				// Fork a child process for heredoc handling
				pid = fork();
				if (pid == -1)
				{
					perror("fork");
					return (0);
				}
				
				if (pid == 0) // Child process
				{
					// Set up special signal handling for heredoc
					setup_heredoc_signals();
					
					// Process heredoc in child
					if (!open_here_doc(in, env))
						exit(1); // Exit with error
					exit(0); // Exit successfully
				}
				else // Parent process
				{
					waitpid(pid, &status, 0);
					
					// Check how child process exited
					if (WIFEXITED(status))
					{
						if (WEXITSTATUS(status) != 0)
						{
							g_exit_status = WEXITSTATUS(status);
							return (0); // Child failed
						}
					}
					else if (WIFSIGNALED(status))
					{
						// Child was terminated by a signal (e.g., SIGINT)
						g_exit_status = 128 + WTERMSIG(status);
						return (0);
					}
					
					// Always restore signal handling in parent
					restore_signals();
				}
			}
			in = in->next;
		}
		tmp = tmp->next;
	}
	return (1);
}

// void	print_list(t_redi_list *in, t_redi_list *out)
// {
// 	static int cmd = 1;
// 	printf("\n");
// 	printf("cmd [ %d ]\n", cmd);
// 	while(in)
// 	{
// 		printf("type : %s\nfile : %s\ncontent : %s\nis_last : %d\n",ft_printtype(in->type), in->file, in->content, in->is_last);
// 		in = in->next;
// 	}
// 	printf("\n");
// 	while(out)
// 	{
// 		printf("type : %s\nfile : %s\ncontent : %s\nis_last : %d\n",ft_printtype(out->type), out->file, out->content, out->is_last);
// 		out = out->next;
// 	}
// 	cmd++;
// }
