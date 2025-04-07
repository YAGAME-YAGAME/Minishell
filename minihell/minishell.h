/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:17:15 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/07 21:02:58 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//--includes FIXME: Make sure no unused includes are present
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

//--macros
//----colors
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"

//--libft functions
# include "libft/libft.h"

//--enums
typedef enum e_token_type
{
	WORD,
	PIPE,
	AND,
	OR,
	L_PARENTHESIS,
	R_PARENTHESIS,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}	t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	SUBSHELL,
}	t_node_type;
//--structs

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	char			quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	char			*input;
	int				pos;
	int				len;
}	t_lexer;

typedef struct s_ast
{
	t_node_type		*node_type;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

//functions
t_list	*ft_envinit(char **env);
void	ft_print_env(t_list *minienv);
char	*ft_getcwd(void);
t_token	*ft_strtok(char *input);
t_lexer	*ft_lexer_init(char *input);

t_token	*get_next_token(t_lexer *lexer);
t_token	*ft_handle_word(t_lexer *lexer);
t_token	*ft_handle_operator(t_lexer *lexer);
t_token	*ft_handle_quotes(t_lexer *lexer, char quote_char);
void	ft_builtins(char *input, t_list *minienv);
t_token	*ft_newtok(t_token *token);
void	ft_tokadd_back(t_token **token_list, t_token *token);
void	free_token(t_token *token);
int		ft_isspecial(char c);
int		ft_isspace(char c);
bool	ft_is_duplicated(t_lexer *lexer, char op[3]);

#endif
