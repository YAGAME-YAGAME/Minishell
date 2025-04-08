/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/08 14:14:18 by abenajib         ###   ########.fr       */
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

void	minishell(char *input, t_list *minienv)
{
	t_token	*token_list;

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
