/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/05 15:57:45 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(char *input, t_list *minienv)
{
	if (input == NULL)
	{
		printf(RED"\n[EOF]\n"RESET);
		exit(0);
	}
	add_history(input);
	ft_builtins(input, minienv);
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
