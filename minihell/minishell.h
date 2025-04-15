/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:17:15 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/15 16:13:41 by abenajib         ###   ########.fr       */
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

// typedef enum e_node_type
// {
// 	NODE_CMD,
// 	NODE_PIPE,
// 	NODE_AND,
// 	NODE_OR,
// 	SUBSHELL,
// }	t_node_type;
//--structs

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	char			quote_type;
	struct s_token	*current;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_lexer
{
	char			*input;
	int				pos;
	int				len;
}	t_lexer;

typedef struct s_redi_list
{
	t_token_type		type;
	char				*file;
	struct s_redi_list	*next;
}	t_redi_list;

typedef struct s_cmdarg
{
	char			*strags;
	bool			is_builtin;
	t_redi_list		*input;
	t_redi_list		*output;
	struct s_cmdarg	*next;
}	t_cmdarg;

t_list		*ft_envinit(char **env);
char		*ft_getcwd(void);
void		ft_builtins(char *input, t_list *minienv);
t_token		*ft_handle_word(t_lexer *lexer);
t_token		*ft_handle_operator(t_lexer *lexer);

t_token		*ft_handle_quotes(t_lexer *lexer, char quote_char);
t_token		*ft_strtok(char *input, t_list *minienv);
t_lexer		*ft_lexer_init(char *input);
t_token		*ft_get_next_token(t_lexer *lexer);
t_token		*ft_newtok(t_token *token);

void		ft_tokadd_back(t_token **token_list, t_token *token);
void		ft_nodeadd_back(t_cmdarg **lst, t_cmdarg *new);
int			ft_isspecial(char c);
int			ft_isspace(char c);
bool		ft_is_duplicated(t_lexer *lexer, char op[3]);

void		ft_print_env(t_list *minienv);
void		ft_print_tokenlist(t_token *token_list);
char		*ft_printtype(t_token_type type);
void		ft_printredi(t_redi_list *redi);
void		ft_printcmd_list(t_cmdarg *cmdarg_list);

void		ft_free_tokenlist(t_token *token_list);
void		ft_free_token(t_token *token);
bool		ft_isredi(t_token *token);
void		ft_parse_word(t_cmdarg **node, t_token *token_list);
t_cmdarg	*ft_get_next_node(t_token *token_list);

t_cmdarg	*ft_newnode(t_cmdarg *node);
t_cmdarg	*ft_init_node(void);
void		ft_expand_variables(char **value, t_list *minienv);
ssize_t		ft_dollar_pos(char *str);
int			ft_get_var_length(char *str);

char		*ft_getenv(char *var, t_list *minienv);
char		*ft_strjoin_free(char *s1, char *s2);
char		*ft_getvar(char *var, t_list *minienv);
void		ft_free_redi_list(t_redi_list *redi);
void		ft_rediradd(t_redi_list **redi, t_redi_list *new);

t_redi_list	*ft_redinew(t_token *token);
void		ft_parse_redi(t_cmdarg **node, t_token *token_list);
void		ft_parse_squote(t_cmdarg **node, t_token *token_list);
void		ft_parse_dquote(t_cmdarg **node, t_token *token_list);
void		ft_free_cmdlist(t_cmdarg *cmdarg_list);
void		ft_free_node(t_cmdarg *node);

#endif
