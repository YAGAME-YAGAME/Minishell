/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 10:42:05 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/23 17:30:16 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//colors
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"

//includes
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <sys/ioctl.h>
# include <pwd.h>

//libft functions
# include "libft.h"

//enums
typedef enum s_token
{
	WORD,					// Represents a command or an argument
	PIPE,					// Represents the pipe '|' operator
	REDI,					// Represents input/output redirection operators ('<', '>', '<<', '>>')
	DOLLAR,					// Represents the dollar sign '$' for variable expansion
	AND,					// Represents the logical AND '&&' operator
	OR,						// Represents the logical OR '||' operator
	SEMICOLON,				// Represents the semicolon ';' operator
	AMPERSAND,				// Represents the background execution '&' operator
	LEFT_PARENTHESIS,		// Represents the left parenthesis '('
	RIGHT_PARENTHESIS,		// Represents the right parenthesis ')'
	LESS_THAN,				// Represents the less than '<' operator for input redirection
	GREATER_THAN, 			// Represents the greater than '>' operator for output redirection
	DOUBLE_LESS_THAN,		// Represents the here-document '<<' operator
	DOUBLE_GREATER_THAN,	// Represents the append '>>' operator
	SINGLE_QUOTE,			// Represents the single quote '\'' for quoted strings
	DOUBLE_QUOTE, 			// Represents the double quote '"' for quoted strings
	BACKSLASH				// Represents the backslash '\' for escape sequences
} t_token;

//typedefs
typedef struct s_list	t_list;
typedef struct s_input	t_input;
typedef struct s_syntax	t_syntax;

//structs
struct s_input
{
	char	*input_str;
	char	**prompt;
	char	**env;
};

struct s_syntax		//this struct is used to initialize the input into t_input then convert it into a linked list t_list then check the syntax if correct
{
	t_input	*input;
	t_list	*lstlexer;
	bool	in_single_quote;
	bool	is_escaped;
	bool	in_double_quotes;
};

struct s_list		// The lexer is linked list contains the string, the type of it so we can handel each one, and pointers to travers
{
	void			*content;
	t_token			type;
	t_token			redi;
	struct s_list	*next;
	struct s_list	*prev;
};

//parsing functions
char	*ft_getcwd(void);
t_list	*ft_lexer(t_syntax *syntax);

#endif // MINISHELL_H
