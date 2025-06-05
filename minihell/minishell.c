/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 16:41:30 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit_status = 0;

/*
 * Calculates the number of tokens in a linked list of tokens.
 * Traverses the token list from the given starting node to the end,
 * counting each token encountered.
 *
 * @param lst: Pointer to the first token in the linked list
 * @return: Number of tokens in the list, 0 if list is NULL
 */
int	ft_toksize(t_token *lst)
{
	t_token	*p;
	int		len;

	if (lst == NULL)
		return (0);
	p = lst;
	len = 0;
	while (p != NULL)
	{
		p = p->next;
		len++;
	}
	return (len);
}

/*
 * Validates the syntax of a tokenized command line input.
 * Checks for common shell syntax errors including misplaced pipes,
 * redirection operators without files, and single redirection tokens.
 * Sets global exit status to 258 on syntax errors and
 * prints appropriate error messages.
 *
 * @param token_list: Pointer to the first token in the command line
 * @return: 0 if syntax is valid, -1 if syntax errors are found
 */
int	ft_check_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	if (ft_toksize(token_list) == 1 && ft_isredi(current))
		return (printf(RED SYNTAX_ERROR RESET), g_exit_status = 258, -1);
	while (current)
	{
		if (ft_pipeerrors(current))
			return (printf(RED PIPE_ERROR RESET), g_exit_status = 258, -1);
		if (ft_redierrors(current))
			return (printf(RED SYNTAX_ERROR RESET), g_exit_status = 258, -1);
		current = current->next;
	}
	return (0);
}

/*
 * Parses a list of tokens into a structured command argument list.
 * Expands environment variables in tokens (except heredoc delimiters),
 * then converts the flat token list into a linked list of command nodes
 * where each node represents a complete command with its arguments and
 * redirections.
 *
 * @param token_list: Linked list of tokens from lexical analysis
 * @param minienv: Environment variables list for variable expansion
 * @return: Linked list of command argument structures, NULL if no tokens
 */
t_cmdarg	*ft_parser(t_token *token_list, t_list *minienv)
{
	t_cmdarg	*cmdarg_list;
	t_cmdarg	*node;
	t_token		*tmp;

	if (!token_list)
		return (NULL);
	cmdarg_list = NULL;
	tmp = token_list;
	while (tmp)
	{
		if (!(tmp->type == WORD && tmp->prev && tmp->prev->type == HEREDOC))
			ft_expand_variables(&tmp, minienv);
		tmp = tmp->next;
	}
	token_list->current = token_list;
	node = ft_get_next_node(token_list);
	while (node)
	{
		if (node)
			ft_nodeadd_back(&cmdarg_list, ft_newnode(node));
		freeall(node->cmd, node->cmdsize);
		free(node);
		node = ft_get_next_node(token_list);
	}
	return (cmdarg_list);
}

void	minishell(char *input, t_list **minienv)
{
	t_token		*token_list;
	t_cmdarg	*cmdarg_list;

	if (input == NULL)
	{
		ft_cmd_error(NULL, RED "[EOF]\n" RESET, 0);
		return ;
	}
	if (input[0] == '\0')
		return ;
	add_history(input);
	token_list = ft_strtok(input, *minienv);
	if (ft_check_syntax(token_list) == -1)
		return (ft_free_tokenlist(token_list));
	cmdarg_list = ft_parser(token_list, *minienv);
	if (!check_here_doc(cmdarg_list, *minienv))
		return (ft_cleaner(token_list, cmdarg_list));
	if (check_builtin(cmdarg_list, minienv) == 1)
		return (ft_cleaner(token_list, cmdarg_list));
	if (!execution(cmdarg_list, *minienv))
		return (ft_cleaner(token_list, cmdarg_list));
	ft_cleaner(token_list, cmdarg_list);
}

/*
 * Entry point of the minishell program.
 * Initializes signal handlers, validates command line arguments,
 * sets up the environment,
 * and enters the main interactive loop where it continuously reads user input
 * and processes commands until the program is terminated.
 *
 * @param ac: Argument count (must be 1, no arguments expected)
 * @param av: Argument vector (unused)
 * @param env: System environment variables array
 * @return: 0 on normal termination, 1 if incorrect arguments provided
 * Side effects: Enters infinite loop, handles signals, manages readline
 */
int	main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*input;

	handle_signals();
	(void)av;
	if (ac != 1)
		return (printf(YELLOW "\nError: No arguments expected\n" RESET), 1);
	else
	{
		minienv = ft_envinit(env);
		while (1)
		{
			input = readline(GREEN "minishell-1.0$ " RESET);
			minishell(input, &minienv);
			free(input);
		}
	}
	return (0);
}

// void	ll(void)
// {
// 	system("leaks -q minishell");
// }
// atexit(ll);
// ft_print_tokenlist(token_list);
// ft_printcmd_list(cmdarg_list);
