/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:42:01 by abenajib          #+#    #+#             */
/*   Updated: 2025/03/16 14:19:00 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Minishell.h"

t_parse	*ft_treenew(t_list *node)
{
	t_parse	*new;

	if (!node)
		return (NULL);
	new = (t_parse *)malloc(sizeof(t_parse));
	if (!new)
		return (NULL);
	new->node = node;
	new->left = NULL;
	new->right = NULL;
	new->type = node->type;
	return (new);
}

t_list	*ft_pipepos(t_list *current)
{
	while (current)
	{
		if (ft_strcmp("|", current->content) == 0)
			return (current);
		current = current->prev;
	}
	return (NULL);
}

t_parse *ft_tree(t_list *lexer)
{
	
}

t_parse	*ft_lst2tree(t_list *lexer)
{
	t_parse	*root;
	t_list *current;

	current = ft_pipepos(ft_lstlast(lexer));
	root = NULL;
	if (!current)
		printf("NO PIPE");
	else
		root = ft_tree(lexer);
	return (root);
}

void	ft_parse(t_minishell *minishell)
{
	// TODO: create a linked list (t_list) containing [string, t_token, next, prev]
	t_list	*lstlexer;
	lstlexer = ft_lexer(minishell->input);
	ft_print_t_list(lstlexer);



	minishell->tree = ft_lst2tree(lstlexer);

	ft_lstclear(&lstlexer, free);






	// TODO: check for quotes
		// TODO: check that each single/double quote is closed

	// TODO: check for parentheses
		// TODO: check the right number order of parentheses left then right and that they are closed

	// TODO: check for variables
	// TODO: check for syntax errors
	// TODO: check for pipes
	// TODO: check for redirections
	// TODO: check for semicolons

	// TODO: create a tree (t_parse) containing [t_token, {cmd, args, ...}, left, right]
	// TODO: create the tree
	// TODO: free the linked list
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;

	if (argc != 1 || !argv)
	{
		printf("Usage: ./Minishell\n");
		exit(1);
	}
	ft_init_minishell(&minishell, envp); // FIXME: this could lead to a memory leak for minishell->env
	while (1)
	{
		minishell.cwd = ft_getcwd();
		minishell.input = readline(minishell.cwd);
		if (!minishell.input)
			ft_eof(&minishell);
		if (*(minishell.input))
		{
			add_history(minishell.input);
			ft_parse(&minishell);
			// if (minishell.syntax_error)
			// 	continue ;
			// if (minishell.tree)
			// ft_exec(&minishell);
		}
		ft_free_init(&minishell);
	}
}
