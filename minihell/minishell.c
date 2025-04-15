/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/15 16:39:35 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_check_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (current->next == NULL || current->next->type == PIPE)
			{
				printf(RED"syntax error near unexpected token `|'\n"RESET);
				return ;
			}
			if (current->prev == NULL || current->prev->type == PIPE)
			{
				printf(RED"syntax error near unexpected token `|'\n"RESET);
				return ;
			}
		}
		current = current->next;
	}
}

t_cmdarg	*ft_parser(t_token *token_list)
{
	t_cmdarg	*cmdarg_list;
	t_cmdarg	*node;

	cmdarg_list = NULL;
	token_list->current = token_list;
	node = ft_get_next_node(token_list);
	while (node)
	{
		if (node)
			ft_nodeadd_back(&cmdarg_list, ft_newnode(node));
		node = ft_get_next_node(token_list);
	}
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
	ft_builtins(input, minienv);
	token_list = ft_strtok(input, minienv);
	ft_check_syntax(token_list);
	ft_print_tokenlist(token_list);
	cmdarg_list = ft_parser(token_list);
	ft_printcmd_list(cmdarg_list);
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

	// atexit(leak_check);
	(void)av;
	if (ac != 1)
		return (perror(YELLOW"Error: No arguments expected"RESET), 1);
	else
	{
		printf(GREEN"Welcome to the Minishell!\n"RESET);
		while (1)
		{
			minienv = ft_envinit(env);
			cwd = ft_getcwd();
			input = readline(cwd);
			minishell(input, minienv);
			free(cwd);
			free(input);
			ft_lstclear(&minienv, free);
		}
		return (0);
	}
}
