/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:42:01 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/20 22:09:40 by abenajib         ###   ########.fr       */
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
	if (ft_strcmp(input_s->prompt[0], "cd") == 0)
	{
		if (!input_s->prompt[1])
			chdir(getenv("HOME"));
		else
			chdir(input_s->prompt[1]); // TODO: handle the case where the directory does not exist;
	}
	else if (ft_strcmp(input_s->prompt[0], "exit") == 0)
	{
		freeall(input_s->prompt, countwords(input_s->input, " \t\n\r\a\v\f"));
		free(input_s->input);
		exit(0);
	}
}

void	ft_parse_input(t_input **input_s)
{
	if ((*input_s)->input == NULL || !*(*input_s)->input)
		return ;
	(*input_s)->prompt = ft_split((*input_s)->input, " \t\n\r\a\v\f");
	if (!(*input_s)->prompt)
		return ;
	ft_builtin(*input_s);
	freeall((*input_s)->prompt, countwords((*input_s)->input, " \t\n\r\a\v\f"));
}

void	ft_handle_input(t_input *input_s)
{
	if (input_s->input == NULL)
	{
		printf(RED"\n[EOF]\n"RESET);
		free(input_s->input);
		exit(0);
	}
	if (*(input_s->input))
	{
		add_history(input_s->input);
		ft_parse_input(&input_s);
	}
}

/*
--input--

--prompt--
ls -la
cat -e
wc -l
while(splitlen(prompt))
	pipe -> fork -> split(prompt[i], "WhiteS") -> check path -> execve

*/

// void	ft_execute(t_input *input_s)
// {
// 	//TODO: after parsing the input, find the path of the command and execute it
// }

void	leaks(void)
{
	system("leaks -q Minishell");
}

int	main(void)
{
	atexit(leaks);
	t_input	input_s;
	char	*dir;

	while (1)
	{
		dir = ft_getcwd();
		input_s.input = readline(dir);
		input_s.prompt = NULL;
		ft_handle_input(&input_s);
		if (dir)
			free(dir);
		//ft_execute(&input_s); //TODO: implement the execution of the command
		free(input_s.input);
	}
	return (0);
}
