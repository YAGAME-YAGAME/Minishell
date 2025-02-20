/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:42:01 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/20 12:35:07 by abenajib         ###   ########.fr       */
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

void	ft_builtin(t_input *input_s)
{
	int	i;

	i = 0;
	if (ft_strcmp(input_s->prompt[0], "cd") == 0)
	{
		if (!input_s->prompt[1])
			chdir(getenv("HOME"));
		else
			chdir(input_s->prompt[1]);
	}
	else if (ft_strcmp(input_s->prompt[0], "exit") == 0)
	{
		freeall(input_s->prompt, countwords(input_s->input, " \t\n\r\a\v\f"));
		exit(0);
	}
}

void	ft_parse_input(char *input)
{
	t_input	input_s;

	if (input == NULL || !*input)
		return ;
	input_s.prompt = ft_split(input, " \t\n\r\a\v\f");
	if (!input_s.prompt)
		return ;
	ft_builtin(&input_s);
	freeall(input_s.prompt, countwords(input, " \t\n\r\a\v\f"));
}

void	ft_handle_input(t_input	*input_s)
{
	if (input_s->input == NULL)
	{
		printf(RED"\n[EOF]\n"RESET);
		exit(0);
	}
	if (*(input_s->input))
	{
		add_history(input_s->input);
		ft_parse_input(input_s->input);
		free(input_s->input);
	}
}

void	leaks(void)
{
	system("leaks -q Minishell");
}

int	main(void)
{
	atexit(leaks);
	t_input	input_s;
	char	*prompt;

	while (1)
	{
		prompt = ft_getcwd();
		input_s.input = readline(prompt);
		input_s.prompt = NULL;
		ft_handle_input(&input_s);
		if (prompt)
			free(prompt);
	}
	return (0);
}
