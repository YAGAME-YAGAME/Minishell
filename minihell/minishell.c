/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/25 21:43:34 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// Global variable for exit status
int g_exit_status = 0;

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    g_exit_status = 130;  // Standard exit code for SIGINT (Ctrl+C)
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

bool	ft_rediErrors(t_token *current)
{
	return (ft_isredi(current) && (current->next == NULL
		|| current->next->type != WORD || ft_isredi(current->next)));
}

bool	ft_pipeErrors(t_token *current)
{
	return (current->type == PIPE && ((current->next == NULL
		|| current->next->type == PIPE || ft_isredi(current->next)
		|| current->prev == NULL || ft_isredi(current->prev))));
}

int	ft_check_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	while (current)
	{
		if (ft_pipeErrors(current))
			return (printf(RED"syntax error near unexpected token `|'\n"RESET), -1);
		if (ft_rediErrors(current))
			return (printf(RED"syntax error near unexpected token 'REDI'\n"RESET), -1);
		current = current->next;
	}
	return (0);
}

t_cmdarg	*ft_parser(t_token *token_list)
{
	t_cmdarg	*cmdarg_list;
	t_cmdarg	*node;

	if (!token_list)
		return (NULL);
	cmdarg_list = NULL;
	token_list->current = token_list;
	node = ft_get_next_node(token_list);
	while (node)
	{
		if (node)
			ft_nodeadd_back(&cmdarg_list, ft_newnode(node));
		free(node);
		node = ft_get_next_node(token_list);
	}
	free(node);
	return (cmdarg_list);
}

void	ft_printcmd_list(t_cmdarg *cmdarg_list)
{
	t_cmdarg	*tmp;

	tmp = cmdarg_list;
	printf("\nCMD Parser:\n");
	while (tmp)
	{
		printf("-------------------------------------\n");
		printf("Command: [%s]\n", tmp->strags);
		if (tmp->input)
			ft_printredi(tmp->input);
		if (tmp->output)
			ft_printredi(tmp->output);
		tmp = tmp->next;
		printf("-------------------------------------\n");
	}
}

void	minishell(char *input, t_list *minienv)
{
	t_token		*token_list;
	t_cmdarg	*cmdarg_list;

	if (input == NULL)
	{
		printf(RED"\n[EOF]"RESET);
		printf(RESET"\n"RESET);
		exit(0);
	}
	if (input[0] == '\0')
		return ;
	add_history(input);
	token_list = ft_strtok(input, minienv);
	// ft_print_tokenlist(token_list);
	if (ft_check_syntax(token_list) == -1)
	{
		ft_free_tokenlist(token_list);
		return ;
	}

	cmdarg_list = ft_parser(token_list);
	// ft_printcmd_list(cmdarg_list);

	check_here_doc(cmdarg_list, minienv);

	if(check_builtin(cmdarg_list, minienv, input) == 1)
		return ;

	if(!execution(cmdarg_list, minienv))
		return ;

	ft_free_tokenlist(token_list);
	ft_free_cmdlist(cmdarg_list);
}

// void	leak_check(void)
// {
// 	system("leaks -q minishell");
// }

int	main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*input;
	char	*cwd;

	signal(SIGINT, handle_sigint); // Handle Ctrl+C
	// atexit(leak_check);
	(void)av;
	if (ac != 1)
		return (perror(YELLOW"Error: No arguments expected"RESET), 1);
	else
	{
		printf(GREEN"Welcome to the Minishell!\n\n"RESET);
		minienv = ft_envinit(env);
		while (1)
		{
			cwd = ft_getcwd(minienv);
			input = readline(cwd);
			minishell(input, minienv);
			free(input);
			free(cwd);
		}
		ft_lstclear(&minienv, free);
		return (0);
	}
	ft_free_list(&minienv);
}
