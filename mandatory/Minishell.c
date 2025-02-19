/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:42:01 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/19 15:09:05 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_parse_input(char *input)
{
	int		i;
	char	**args;

	if (input == NULL || !*input)
		return ;
	args = ft_split(input, " \t\n\r\a\v\f");
	i = 0;
	if (ft_strcmp(args[0], "cd") == 0)
	{
		if (!args[1])
			chdir(getenv("HOME"));
		else
			chdir(args[1]);
	}
	else if (ft_strcmp(args[0], "exit") == 0)
	{
		freeall(args, i);
		exit(0);
	}
	freeall(args, i);
}

int	main(void)
{
	char	*input;
	char	*prompt;

	while (1)
	{
		prompt = ft_getcwd();
		input = readline(prompt);
		if (input == NULL)
		{
			printf(RED"\n[EOF]\n"RESET);
			exit(0);
		}
		if (*input)
		{
			add_history(input);
			ft_parse_input(input);
			free(input);
		}
		if (prompt)
			free(prompt);
	}
	return (0);
}
