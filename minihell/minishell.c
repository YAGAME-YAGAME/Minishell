/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/09 18:01:38 by abenajib         ###   ########.fr       */
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

t_cmdarg	*ft_newnode(t_cmdarg *node)
{
	t_cmdarg	*new;

	new = malloc(sizeof(t_cmdarg));
	if (!new)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new->strags = node->strags;
	new->is_builtin = node->is_builtin;
	new->input = node->input;
	new->output = node->output;
	new->next = NULL;
	return (new);
}

void	free_node(t_cmdarg *node)
{
	if (node)
	{
		free(node->strags);
		free(node);
	}
}

t_cmdarg	*ft_parser(t_token *token_list, t_list *minienv)
{
	t_cmdarg	*cmdarg_list;
	t_cmdarg	*node;
	(void)minienv;

	cmdarg_list = NULL;
	node = get_next_node(token_list);
	while (node)
	{
		// if (node)
		// 	ft_nodeadd_back(&cmdarg_list, ft_newnode(node));
		// if (node && node->strags)
		// 	printf("%s\n", node->strags);
		free_node(node);
		node = get_next_node(token_list);
	}
	return (cmdarg_list);
}

void	minishell(char *input, t_list *minienv)
{
	t_token		*token_list;
	t_cmdarg	*cmdarg_list;

	if (input == NULL)
	{
		printf(RED"\n[EOF]\n"RESET);
		exit(0);
	}
	add_history(input);
	ft_builtins(input, minienv);
	token_list = ft_strtok(input);
	ft_check_syntax(token_list);
	// print_tokenlist(token_list);

	if (token_list)
		cmdarg_list = ft_parser(token_list, minienv);
	ft_free_tokenlist(token_list);
}

int	main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*input;
	char	*cwd;

	(void)av;
	if (ac != 1)
		return (perror(YELLOW"Error: No arguments expected"RESET), 1);
	else
	{
		printf(GREEN"Welcome to the Minishell!\n"RESET);
		minienv = ft_envinit(env);
		while (1)
		{
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
