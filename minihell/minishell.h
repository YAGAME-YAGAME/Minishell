/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:17:15 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 20:37:15 by abenajib         ###   ########.fr       */
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

//----colors
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"

//--macros
# define HEREDOC_FILE "/tmp/minishell_heredoc_tmp"
# define UNCLOSED "unexpected EOF while looking for matching quote"

// Global variables
extern int g_exit_status;

// Signal handling and readline functions
void	handle_sigint(int sig);
void	setup_signals(void);
void	init_readline(void);

// Signal handling functions
void	handle_sigint(int sig);
void	handle_heredoc_sigint(int sig);
void	handle_signals(void);
void	setup_heredoc_signals(void);
void	restore_signals(void);

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

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	bool			addSpace;
	bool			variable;
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
	char				*content;
	bool				is_last;
	int 				tmp_fd;
	bool 				is_ambiguous;
	bool				expand;
	bool				variable;
	struct s_redi_list	*next;
}	t_redi_list;

typedef struct s_cmdarg
{
	char			*strags;
	char			**cmd;
	int				cmdSize;
	bool			is_builtin;
	int 			origin_stdout;
	int 			origin_stdin;
	t_redi_list		*input;
	t_redi_list		*output;
	struct s_cmdarg	*next;
}	t_cmdarg;

t_list		*ft_envinit(char **env);
char		*ft_getcwd(t_list *env);
t_token		*ft_handle_word(t_lexer *lexer);
t_token		*ft_handle_operator(t_lexer *lexer);

t_token		*ft_handle_quotes(t_lexer *lexer, char quote_char);
t_token		*ft_strtok(char *input);
t_lexer		*ft_lexer_init(char *input);
t_token		*ft_get_next_token(t_lexer *lexer);
t_token		*ft_newtok(t_token *token);

void		minishell(char *input, t_list **minienv);
int			ft_toksize(t_token *lst);
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
bool		ft_is_cmd(t_token *current);
void		*ft_parse_word(t_cmdarg **node, t_token *token_list);
t_cmdarg	*ft_get_next_node(t_token *token_list);

t_cmdarg	*ft_newnode(t_cmdarg *node);
t_cmdarg	*ft_init_node(void);
void		ft_expand_variables(t_token **token, t_list *minienv);
void		ft_expand_var_in_char(char **value, t_list *minienv);
ssize_t		ft_dollar_pos(char *str);
int			ft_get_varlen(char *str);

char		*ft_getenv(char *var, t_list *minienv);
char		*ft_strjoin_free(char *s1, char *s2);
char		*ft_getvar(char *var, t_list *minienv);
void		ft_free_redi_list(t_redi_list *redi);
void		ft_rediradd(t_redi_list **redi, t_redi_list *new);

t_redi_list	*ft_redinew(t_token *token, bool expand);
void		ft_parse_redi(t_cmdarg **node, t_token *token_list);
void		ft_free_cmdlist(t_cmdarg *cmdarg_list);
void		ft_free_node(t_cmdarg *node);

bool		ft_condition(t_token **token, ssize_t dollar_pos);
char		*ft_expand(t_token **token, t_list *minienv, ssize_t dollar_pos);
char		*ft_temp2(t_token **token, ssize_t dollar_pos);
char		*ft_strjoin_free(char *s1, char *s2);

bool		ft_condition_inchar(char *value, ssize_t dollar_pos);
void		ft_expand_exit_status_inchar(char **value);
char		*ft_expand_inchar(char *value, t_list *minienv, ssize_t dollar_pos);
char		*ft_temp2_inchar(char *value, ssize_t dollar_pos);

//--exec
int			check_here_doc(t_cmdarg *shell, t_list *env);
int			execution(t_cmdarg *shell, t_list *env);
int			count(char *s, char p);
void		free_all(char **bf, int j);
void		ft_error(char *message);
char		**parsing_split(char *s, char p);
char		*find_path( t_list *path);
char		*check_exec(char *p, t_list *env);
void		ft_child(t_cmdarg *current_cmd, t_list *env, int tmp_in, int *p_fd);
char		**get_env(t_list *env);
char		*get_next_line(int fd);
char		*my_strjoin(char *s1, char *s2);
char		*my_strdup(const char *s1);
void 		ft_free_list(t_list **list);
int			size_list(t_cmdarg *node);
char 		*ft_get_pwd(t_list *env);
bool 		is_ambiguous(char *file);
void 		ft_cmd_error(char *cmd_name, char *error, int status);
int 		ft_open_file(char *file, int flag);

//--builtins
void		ft_read_line(int fd, char **line, char *delimiter, t_redi_list *heredoc, t_list *env);
void		init_redi_file(t_cmdarg *shell);
void		ft_update_path(t_list *env, char *new_path, char *old_path);
int			run_built_in(t_cmdarg *shell, t_list **env, char *input);
void		handle_input(t_redi_list *input);
void		handle_output(t_redi_list *output);
int			check_builtin(t_cmdarg *cmdarg_list, t_list **minienv, char *input);
char		**handel_quote(char **cmd);
void		ft_cmd_error(char *cmd_name, char *error, int status);


int			ft_echo(char **cmd, t_cmdarg*env);
int			ft_pwd(t_list **env);
int			ft_unset(char **cmd, t_list **env);
int			ft_exit(char **cmd, t_list **env, char *input);
int			ft_cd(char **cmd, t_list **env);
int			ft_env(t_list **env);
int			ft_export(char **cmd, t_list **env);
// int			ft_clear();
int			is_builtin(char *cmd);
int			ft_set_env(t_list **env);
void		free_dp(char **cmd);
int			remove_env_node(t_list **env_list, t_list *node);
void		ft_reset_std(t_cmdarg *shell);

// --signals
void 		handle_signals();


#endif
