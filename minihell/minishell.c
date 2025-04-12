/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/12 19:01:40 by abenajib         ###   ########.fr       */
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
		return (NULL);
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

void	ft_nodeadd_back(t_cmdarg **lst, t_cmdarg *new)
{
	t_cmdarg	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_cmdarg	*ft_parser(t_token *token_list, t_list *minienv)
{
	t_cmdarg	*cmdarg_list;
	t_cmdarg	*node;
	(void)minienv;

	cmdarg_list = NULL;
	token_list->current = token_list;
	node = get_next_node(token_list, minienv);
	while (node)
	{
		if (node)
			ft_nodeadd_back(&cmdarg_list, ft_newnode(node));
		node = get_next_node(token_list, minienv);
	}
	return (cmdarg_list);
}

void	printcmd_list(t_cmdarg *cmdarg_list)
{
	t_cmdarg	*tmp;

	tmp = cmdarg_list;
	printf("\nCMD Parser:\n");
	while (tmp)
	{
		printf("-------------------------------------\n");
		printf("Command: [%s]\n", tmp->strags);
		if (tmp->input)
			printredi(tmp->input);
		if (tmp->output)
			printredi(tmp->output);
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
		printf(RED"\n[EOF]\n"RESET);
		exit(0);
	}
	add_history(input);
	ft_builtins(input, minienv);
	token_list = ft_strtok(input);
	ft_check_syntax(token_list);
	print_tokenlist(token_list);
	cmdarg_list = ft_parser(token_list, minienv);
	printcmd_list(cmdarg_list);
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
