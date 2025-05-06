/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:05:19 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

/**
 * @brief Checks for redirection syntax errors in the token list
 * @param current The token to check for redirection errors
 * @return true if there is a redirection error, false otherwise
 */
bool	ft_rediErrors(t_token *current)
{
	return (ft_isredi(current) && (current->next == NULL
		|| (current->next->type != WORD && current->next->type != DOUBLE_QUOTE
			&& current->next->type != SINGLE_QUOTE)
			|| ft_isredi(current->next)));
}

/**
 * @brief Checks for pipe syntax errors in the token list
 * @param current The token to check for pipe errors
 * @return true if there is a pipe error, false otherwise
 */
bool	ft_pipeErrors(t_token *current)
{
	return (current->type == PIPE && ((current->next == NULL
		|| current->next->type == PIPE || current->prev == NULL
		|| ft_isredi(current->prev))));
}

/**
 * @brief Calculates the number of tokens in a token list
 * @param lst The token list to count
 * @return The number of tokens in the list
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

/**
 * @brief Validates the syntax of the token list, checking for common errors
 * @param token_list The token list to check
 * @return 0 if syntax is valid, -1 if there are syntax errors
 */
int	ft_check_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	if (ft_toksize(token_list) == 1 && ft_isredi(current))
		return (printf(RED"syntax error near unexpected token `newline'\n"RESET), -1);
	while (current)
	{
		if (ft_pipeErrors(current))
			return (printf(RED"syntax error near unexpected token `|'\n"RESET), -1);
		if (ft_rediErrors(current))
			return (printf(RED"syntax error near unexpected token \n"RESET), -1);
		current = current->next;
	}
	return (0);
}

/**
 * @brief Parses token list into command arguments structure
 * @param token_list The token list to parse
 * @param minienv The environment variables list
 * @return A linked list of command arguments
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
		freeall(node->cmd, node->cmdSize);
		free(node);
		node = ft_get_next_node(token_list);
	}
	return (cmdarg_list);
}

/**
 * @brief Debug function to print the command argument list structure
 * @param cmdarg_list The command argument list to print
 */
void	ft_printcmd_list(t_cmdarg *cmdarg_list)
{
	t_cmdarg	*tmp;

	tmp = cmdarg_list;
	printf("\nCMD Parser:\n");
	while (tmp)
	{
		printf("-------------------------------------\n");
		printf("Command:\n");
		for (int i = 0; i < tmp->cmdSize; i++)
			printf("[%s]\n", tmp->cmd[i]);
		printf("\n");
		printf("Redirections:\n");
		if (tmp->input)
			ft_printredi(tmp->input);
		if (tmp->output)
			ft_printredi(tmp->output);
		tmp = tmp->next;
		printf("-------------------------------------\n\n");
	}
}

/**
 * @brief Frees memory allocated for token list and command argument list
 * @param token_list The token list to free
 * @param cmdarg_list The command argument list to free
 */
void	ft_cleaner(t_token *token_list, t_cmdarg *cmdarg_list)
{
	ft_free_tokenlist(token_list);
	ft_free_cmdlist(cmdarg_list);
}

/**
 * @brief Core function of the shell, processes user input
 * @param input The user input string
 * @param minienv Pointer to the environment variables list
 */
void	minishell(char *input, t_list **minienv)
{
	t_token		*token_list;
	t_cmdarg	*cmdarg_list;

	if (input == NULL)
	ft_cmd_error(NULL, RED"[EOF]\n"RESET, 0);
	if (input[0] == '\0')
		return ;
	add_history(input);
	token_list = ft_strtok(input);
	// ft_print_tokenlist(token_list);
	if (ft_check_syntax(token_list) == -1)
		return (ft_free_tokenlist(token_list));
	cmdarg_list = ft_parser(token_list, *minienv);
	// ft_printcmd_list(cmdarg_list);
	if (!check_here_doc(cmdarg_list, *minienv))
		return (ft_cleaner(token_list, cmdarg_list));
	if(check_builtin(cmdarg_list, minienv, input) == 1)
		return (ft_cleaner(token_list, cmdarg_list));
	if(!execution(cmdarg_list, *minienv))
		return (ft_cleaner(token_list, cmdarg_list));
	ft_cleaner(token_list, cmdarg_list);
}

// /**
//  * @brief Memory leak checker for debugging
//  */
// void	leak_check(void)
// {
// 	system("leaks -q minishell");
// }

/**
 * @brief Main function of the minishell program
 * @param ac Argument count
 * @param av Argument vector
 * @param env Environment variables
 * @return Exit status of the program
 */
int	main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*input;
	char	*cwd;
	// atexit(leak_check);

	handle_signals();
	(void)av;
	if (ac != 1)
		return (printf(YELLOW"\nError: No arguments expected\n"RESET), 1);
	else
	{
		// printf(GREEN"Welcome to the Minishell!\n\n"RESET);
		minienv = ft_envinit(env);
		while (1)
		{
			cwd = ft_getcwd(minienv);
			input = readline(cwd);
			minishell(input, &minienv);
			free(input);
			free(cwd);
		}
		ft_lstclear(&minienv, free);
	}
	ft_free_list(&minienv);
	return (0);
}
