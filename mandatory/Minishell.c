/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:42:01 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/25 16:33:03 by abenajib         ###   ########.fr       */
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
	if (!input_s || !input_s->prompt || !input_s->prompt[0])
		return;
	if (ft_strcmp(input_s->prompt[0], "cd") == 0)
	{
		if (!input_s->prompt[1])
		{
			if (chdir(getenv("HOME")) != 0)
				perror("cd");
		}
		else
		{
			if (chdir(input_s->prompt[1]) != 0)
				perror("cd");
		}
		// TODO: handle the case where the directory does not exist;
	}
	else if (ft_strcmp(input_s->prompt[0], "exit") == 0)
	{
		freeall(input_s->prompt, countwords(input_s->input_str, " \t\n\r\a\v\f"));
		free(input_s->input_str);
		exit(0);
	}
}

void	ft_parse_input(t_syntax *syntax)
{

	if (syntax->input->input_str == NULL || !syntax->input->input_str)
		return ;
	syntax->input->prompt = ft_split(syntax->input->input_str, " \t\n\r\a\v\f");
	if (!syntax->input->prompt)
		return ;
	ft_builtin(syntax->input);
	freeall(syntax->input->prompt, countwords(syntax->input->input_str, " \t\n\r\a\v\f"));
}

t_list	*ft_lexer(t_syntax *syntax)	//tokenize the input into list containing the type of each one
{
	char	*input;
	int		i;
	char	buffer[4096];

	input = syntax->input->input_str;	//point on the input string to simplify reading
	i = 0;
	while (*input)	//loop through the input string
	{
		if (ft_strchr(" \t\n\r\a\v\f", *input))	//skip spaces
		{
			input++;
			continue;
		}
		if (ft_strchr("|<>&;\\)\'(\"", *input))	//if we find a token
		{
			if (i > 0)	//if we have a word before the token
			{
				buffer[i] = '\0';
				ft_lstadd_back(&syntax->lstlexer, ft_lstnew(ft_strdup(buffer)));	//add the word to the list
				i = 0;
			}
			while (*input && ft_strchr("<>|&;\\)\'(\"", *input))	//handle multi-character tokens
			{
				buffer[i++] = *input++;
				if (ft_strchr(";\\)\'(\"", *input) && i > 0)
					break;
			}
			buffer[i] = '\0';
			ft_lstadd_back(&syntax->lstlexer, ft_lstnew(ft_strdup(buffer)));	//add the token to the list
			i = 0;
		}
		else	//if we have a word
		{
			while (*input && !ft_strchr("|<>&;\\)\'(\"", *input) && !ft_strchr(" \t\n\r\a\v\f", *input))	//read the word
				buffer[i++] = *input++;
			buffer[i] = '\0';
			ft_lstadd_back(&syntax->lstlexer, ft_lstnew(ft_strdup(buffer)));
			i = 0;
		}
	}
	return (syntax->lstlexer);	//return the list
}

bool	ft_cmp(t_list *current, t_token token) // check each token if it's equal to the current content
{
	if (token == PIPE && ft_strcmp((const char*)(current->content), "|") == 0)
		return (true);
	else if (token == REDI && (ft_strcmp((const char*)(current->content), "<") == 0 || ft_strcmp((const char*)(current->content), ">") == 0 || ft_strcmp((const char*)(current->content), "<<") == 0 || ft_strcmp((const char*)(current->content), ">>") == 0))
		return (true);
	else if (token == DOLLAR && ft_strcmp((const char*)(current->content), "$") == 0)
		return (true);
	else if (token == AND && ft_strcmp((const char*)(current->content), "&&") == 0)
		return (true);
	else if (token == OR && ft_strcmp((const char*)(current->content), "||") == 0)
		return (true);
	else if (token == SEMICOLON && ft_strcmp((const char*)(current->content), ";") == 0)
		return (true);
	else if (token == AMPERSAND && ft_strcmp((const char*)(current->content), "&") == 0)
		return (true);
	else if (token == LEFT_PARENTHESIS && ft_strcmp((const char*)(current->content), "(") == 0)
		return (true);
	else if (token == RIGHT_PARENTHESIS && ft_strcmp((const char*)(current->content), ")") == 0)
		return (true);
	else if (token == LESS_THAN && ft_strcmp((const char*)(current->content), "<") == 0)
		return (true);
	else if (token == GREATER_THAN && ft_strcmp((const char*)(current->content), ">") == 0)
		return (true);
	else if (token == DOUBLE_LESS_THAN && ft_strcmp((const char*)(current->content), "<<") == 0)
		return (true);
	else if (token == DOUBLE_GREATER_THAN && ft_strcmp((const char*)(current->content), ">>") == 0)
		return (true);
	else if (token == SINGLE_QUOTE && ft_strcmp((const char*)(current->content), "'") == 0)
		return (true);
	else if (token == DOUBLE_QUOTE && ft_strcmp((const char*)(current->content), "\"") == 0)
		return (true);
	else if (token == BACKSLASH && ft_strcmp((const char*)(current->content), "\\") == 0)
		return (true);
	return (false);
}

void	ft_set_tokens(t_list *current) // set the type of each token
{
	if (ft_cmp(current, PIPE))
			current->type = PIPE;
		else if (ft_cmp(current, REDI))
			current->type = REDI;
		else if (ft_cmp(current, DOLLAR))
			current->type = DOLLAR;
		else if (ft_cmp(current, AND))
			current->type = AND;
		else if (ft_cmp(current, OR))
			current->type = OR;
		else if (ft_cmp(current, SEMICOLON))
			current->type = SEMICOLON;
		else if (ft_cmp(current, AMPERSAND))
			current->type = AMPERSAND;
		else if (ft_cmp(current, LEFT_PARENTHESIS))
			current->type = LEFT_PARENTHESIS;
		else if (ft_cmp(current, RIGHT_PARENTHESIS))
			current->type = RIGHT_PARENTHESIS;
		else if (ft_cmp(current, LESS_THAN))
			current->type = LESS_THAN;
		else if (ft_cmp(current, GREATER_THAN))
			current->type = GREATER_THAN;
		else if (ft_cmp(current, DOUBLE_LESS_THAN))
			current->type = DOUBLE_LESS_THAN;
		else if (ft_cmp(current, DOUBLE_GREATER_THAN))
			current->type = DOUBLE_GREATER_THAN;
		else if (ft_cmp(current, SINGLE_QUOTE))
			current->type = SINGLE_QUOTE;
		else if (ft_cmp(current, DOUBLE_QUOTE))
			current->type = DOUBLE_QUOTE;
		else if (ft_cmp(current, BACKSLASH))
			current->type = BACKSLASH;
		else
			current->type = WORD;
}

void	print_type(t_token type)
{
	if (type == WORD)
		printf("<WORD> ");
	else if (type == PIPE)
		printf("<PIPE> ");
	else if (type == REDI)
		printf("<REDI> ");
	else if (type == DOLLAR)
		printf("<DOLLAR> ");
	else if (type == AND)
		printf("<AND> ");
	else if (type == OR)
		printf("<OR> ");
	else if (type == SEMICOLON)
		printf("<SEMICOLON> ");
	else if (type == AMPERSAND)
		printf("<AMPERSAND> ");
	else if (type == LEFT_PARENTHESIS)
		printf("<LEFT_PARENTHESIS> ");
	else if (type == RIGHT_PARENTHESIS)
		printf("<RIGHT_PARENTHESIS> ");
	else if (type == LESS_THAN)
		printf("<LESS_THAN> ");
	else if (type == GREATER_THAN)
		printf("<GREATER_THAN> ");
	else if (type == DOUBLE_LESS_THAN)
		printf("<DOUBLE_LESS_THAN> ");
	else if (type == DOUBLE_GREATER_THAN)
		printf("<DOUBLE_GREATER_THAN> ");
	else if (type == SINGLE_QUOTE)
		printf("<SINGLE_QUOTE> ");
	else if (type == DOUBLE_QUOTE)
		printf("<DOUBLE_QUOTE> ");
	else if (type == BACKSLASH)
		printf("<BACKSLASH> ");
}

bool	ft_check_syntax(t_syntax *syntax)
{
	t_list	*current;

	current = syntax->lstlexer;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (!current->next || !current->prev)
			{
				printf(RED"syntax error near unexpected token '|'\n"RESET);
				return (false);
			}
		}
		current = current->next;
	}
	return (true);
}

bool	ft_check_tokens(t_syntax *syntax)
{
	t_list	*current;

	current = syntax->lstlexer; // now we have the list so far with all the input splited without tokens, so let set some types
	while (current)
	{
		ft_set_tokens(current);
		current = current->next;
	}
	if (!syntax->lstlexer)
		return (false);
	// if (!ft_check_syntax(syntax))
	// 	return (false);

	printf("\n\nlexer output:\n");
	t_list *hold = syntax->lstlexer;
	while (syntax->lstlexer)
	{
		printf("[%s]->", (char *)syntax->lstlexer->content);
		syntax->lstlexer = syntax->lstlexer->next;
		if (!syntax->lstlexer)
			printf("[NULL]");
	}
	printf("\n");
	syntax->lstlexer = hold;
	while (syntax->lstlexer)
	{
		if (syntax->lstlexer->type == WORD)
			printf("<%s> ", "WORD");
		else if (syntax->lstlexer->type == PIPE)
			printf("<%s> ", "PIPE");
		else if (syntax->lstlexer->type == REDI)
			printf("<%s> ", "REDI");
		else if (syntax->lstlexer->type == DOLLAR)
			printf("<%s> ", "DOLLAR");
		else if (syntax->lstlexer->type == AND)
			printf("<%s> ", "AND");
		else if (syntax->lstlexer->type == OR)
			printf("<%s> ", "OR");
		else if (syntax->lstlexer->type == SEMICOLON)
			printf("<%s> ", "SEMICOLON");
		else if (syntax->lstlexer->type == AMPERSAND)
			printf("<%s> ", "AMPERSAND");
		else if (syntax->lstlexer->type == LEFT_PARENTHESIS)
			printf("<%s> ", "LEFT_PARENTHESIS");
		else if (syntax->lstlexer->type == RIGHT_PARENTHESIS)
			printf("<%s> ", "RIGHT_PARENTHESIS");
		else if (syntax->lstlexer->type == LESS_THAN)
			printf("<%s> ", "LESS_THAN");
		else if (syntax->lstlexer->type == GREATER_THAN)
			printf("<%s> ", "GREATER_THAN");
		else if (syntax->lstlexer->type == DOUBLE_LESS_THAN)
			printf("<%s> ", "DOUBLE_LESS_THAN");
		else if (syntax->lstlexer->type == DOUBLE_GREATER_THAN)
			printf("<%s> ", "DOUBLE_GREATER_THAN");
		else if (syntax->lstlexer->type == SINGLE_QUOTE)
			printf("<%s> ", "SINGLE_QUOTE");
		else if (syntax->lstlexer->type == DOUBLE_QUOTE)
			printf("<%s> ", "DOUBLE_QUOTE");
		else if (syntax->lstlexer->type == BACKSLASH)
			printf("<%s> ", "BACKSLASH");
		syntax->lstlexer = syntax->lstlexer->next;
	}
	printf("\n");
	printf("\n");
	return (true);
}

bool	ft_handle_input(t_syntax *syntax)
{
	if (syntax->input->input_str == NULL) // check if EOF
	{
		printf(RED"\n[EOF]\n"RESET);
		free(syntax->input->input_str);
		exit(0);
	}
	if (*(syntax->input->input_str)) // if not add the cmd to the history then pass it to the lexer
	{
		add_history(syntax->input->input_str);
		syntax->lstlexer = ft_lexer(syntax);
		if (!syntax->lstlexer)
			return (false);
		if (!ft_check_tokens(syntax))
			return (false);
		ft_builtin(syntax->input);
	}
	return (true);
}

void	leaks(void)
{
	system("leaks -q Minishell");
}

int	main(void)
{
	// atexit(leaks);
	t_input		input_s;
	t_syntax	syntax;
	char	*dir;

	syntax.input = &input_s;
	while (1)
	{
		dir = ft_getcwd();
		input_s.input_str = readline(dir);
		input_s.prompt = NULL;
		if (!ft_handle_input(&syntax))
			continue ;
		if (dir)
			free(dir);
		free(input_s.input_str);
	}
	return (0);
}
