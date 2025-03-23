/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:42:01 by abenajib          #+#    #+#             */
/*   Updated: 2025/03/20 05:21:38 by abenajib         ###   ########.fr       */
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

void	ft_pipepos(t_list **current) // Working as expected
{
	if (ft_strcmp("|", (*current)->content) == 0)
		(*current) = (*current)->prev;
	while (*current != NULL)
	{
		if (ft_strcmp("|", (*current)->content) == 0)
			return ;
		(*current) = (*current)->prev;
	}
}


// Add helper to check if node is a pipe
int is_pipe(t_list *node)
{
	return (node && ft_strcmp(node->content, "|") == 0);
}

t_parse *ft_tree(t_list *lexer, t_list **current)
{
    if (!*current || !is_pipe(*current))
        return NULL; // Base case: stop if no pipe

    t_parse *node = ft_treenew(*current);
    t_list *prev_pipe = *current;

    // Move to previous pipe (or to the leftmost command if no pipe)
    ft_pipepos(current);

    if (*current && is_pipe(*current))
    {
        node->left = ft_tree(lexer, current);
        node->right = ft_treenew(prev_pipe->next);
		prev_pipe->prev->next = NULL; // remove the pipe from the list
		prev_pipe->next->prev = NULL; // remove the pipe from the list
		prev_pipe = NULL;
    }
    else
    {
        // *current now points to the leftmost command.
        node->left = ft_treenew(lexer);
        node->right = ft_treenew(prev_pipe->next);
		prev_pipe->prev->next = NULL; // remove the pipe from the list
		prev_pipe->next->prev = NULL; // remove the pipe from the list
		prev_pipe = NULL;
    }
    return node;
}


void	ft_print_tree(t_parse *root)
{
	if (!root)
	{
		printf("(empty tree)\n");
		return ;
	}
	printf("Parse Tree:\n");
	ft_print_tree_visual(root);
}

t_parse	*ft_lst2tree(t_list *lexer)
{
	t_parse	*root;
	t_list *current;

	current = ft_lstlast(lexer); // last node
	ft_pipepos(&current);		// current -> last pipe,  if no pipe current -> NULL
	root = NULL;
	if (!current)
		printf("NO PIPE");
	else
	{
		root = ft_tree(lexer, &current);
		ft_print_tree(root);
	}
	return (root);
}

// this is a comment

void	ft_parse(t_minishell *minishell)
{
	// TODO: create a linked list (t_list) containing [string, t_token, next, prev]
	t_list	*lstlexer;
	lstlexer = ft_lexer(minishell->input);
	ft_print_t_list(lstlexer);



	minishell->tree = ft_lst2tree(lstlexer);
	// ft_print_tree(minishell->tree);

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
