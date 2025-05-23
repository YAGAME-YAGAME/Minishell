/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:17:15 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/23 04:24:51 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <curses.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

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
# define UNCLOSED "unexpected EOF while looking for matching quote\n"
# define EXIT_ERROR "exit: numeric argument required"
# define PIPE_ERROR "syntax error near unexpected token `|'\n"
# define SYNTAX_ERROR "syntax error near unexpected token `newline'\n"

# define CD_HOME_ERR "minishell :cd: HOME not set\n"
# define CD_OLDPWD_ERR "minishell :cd: OLDPWD not set\n"
// Global variables
extern int				g_exit_status;

// Signal handling and readline functions
void					handle_sigint(int sig);
// void					setup_signals(void);
// void					init_readline(void);

// Signal handling functions
void					handle_sigint(int sig);
void					handle_heredoc_sigint(int sig);
void					handle_signals(void);
void					setup_heredoc_signals(void);
void					restore_signals(void);

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
}						t_token_type;

//--structs
typedef struct s_list_heredoc
{
	char				*delimiter;
	char				*line;
	int					fd;
}						t_list_heredoc;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	bool				addspace;
	bool				variable;
	struct s_token		*current;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_lexer
{
	char				*input;
	int					pos;
	int					len;
}						t_lexer;

typedef struct s_redi_list
{
	t_token_type		type;
	char				*file;
	char				*content;
	bool				is_last;
	int					tmp_fd;
	bool				is_ambiguous;
	bool				expand;
	bool				variable;
	int					heredoc_fd;
	struct s_redi_list	*next;
}						t_redi_list;

typedef struct s_cmdarg
{
	char				**cmd;
	int					cmdsize;
	bool				is_builtin;
	int					origin_stdout;
	int					origin_stdin;
	t_redi_list			*input;
	t_redi_list			*output;
	struct s_cmdarg		*next;
}						t_cmdarg;

t_list					*ft_envinit(char **env);
int						ft_parse_env_var(char *env_var, char **key,
							char **value);
char					*ft_getcwd(t_list *env);
void					ft_init_token_fields(t_token *token);
t_token					*ft_handle_word(t_lexer *lexer);
t_token					*ft_handle_operator(t_lexer *lexer, bool *heredoc);

t_token					*ft_handle_quotes(t_lexer *lexer, char quote_char);
t_token					*ft_strtok(char *input, t_list *minienv);
t_lexer					*ft_lexer_init(char *input);
t_token					*ft_get_next_token(t_lexer *lexer, t_list *minienv,
							bool *heredoc);
t_token					*ft_newtok(t_token *token);
void					ft_freeee(char *temp2, char *exp);
void					minishell(char *input, t_list **minienv);
int						ft_toksize(t_token *lst);
void					ft_tokadd_back(t_token **token_list, t_token *token);
void					ft_nodeadd_back(t_cmdarg **lst, t_cmdarg *new);
int						ft_isspecial(char c);
int						ft_isspace(char c);
bool					ft_is_duplicated(t_lexer *lexer, char op[3]);

void					ft_print_env(t_list *minienv);
void					ft_print_tokenlist(t_token *token_list);
char					*ft_printtype(t_token_type type);
void					ft_printredi(t_redi_list *redi);
void					ft_printcmd_list(t_cmdarg *cmdarg_list);

void					ft_free_tokenlist(t_token *token_list);
void					ft_free_token(t_token *token);
bool					ft_isredi(t_token *token);
bool					ft_is_cmd(t_token *current);
void					*ft_parse_word(t_cmdarg **node, t_token *token_list);
t_cmdarg				*ft_get_next_node(t_token *token_list);

t_cmdarg				*ft_newnode(t_cmdarg *node);
t_cmdarg				*ft_init_node(void);
void					ft_expand_variables(t_token **token, t_list *minienv);
void					ft_expand_var_in_char(char **value, t_list *minienv);
ssize_t					ft_dollar_pos(char *str);
int						ft_get_varlen(char *str);

char					*ft_getenv(char *var, t_list *minienv);
char					*ft_strjoin_free(char *s1, char *s2);
char					*ft_getvar(char *var, t_list *minienv);
void					ft_free_redi_list(t_redi_list *redi);
void					ft_rediradd(t_redi_list **redi, t_redi_list *new);

t_redi_list				*ft_redinew(t_token *token, bool expand);
void					ft_parse_redi(t_cmdarg **node, t_token *token_list);
void					ft_free_cmdlist(t_cmdarg *cmdarg_list);
void					ft_free_node(t_cmdarg *node);

bool					ft_condition(t_token **token, ssize_t dollar_pos);
char					*ft_expand(t_token **token, t_list *minienv,
							ssize_t dollar_pos);
char					*ft_temp2(t_token **token, ssize_t dollar_pos);
char					*ft_strjoin_free(char *s1, char *s2);

bool					ft_condition_inchar(char *value, ssize_t dollar_pos);
void					ft_expand_exit_status_inchar(char **value);
char					*ft_expand_inchar(char *value, t_list *minienv,
							ssize_t dollar_pos);
char					*ft_temp2_inchar(char *value, ssize_t dollar_pos);

// minishell_utils
bool					ft_redierrors(t_token *current);
bool					ft_pipeerrors(t_token *current);
void					ft_cleaner(t_token *token_list, t_cmdarg *cmdarg_list);

//--exec
int						size_list(t_cmdarg *node);
int						check_here_doc(t_cmdarg *shell, t_list *env);
int						execution(t_cmdarg *shell, t_list *env);
int						count(char *s, char p);
void					free_all(char **bf, int j);
void					ft_error(char *message);
char					**parsing_split(char *s, char p);
char					*find_path(t_list *path);
char					*check_exec(char *p, t_list *env);
void					ft_child(t_cmdarg *current_cmd, t_list *env, int tmp_in,
							int *p_fd);
char					**get_env(t_list *env);
char					*get_next_line(int fd);
char					*my_strjoin(char *s1, char *s2);
char					*my_strdup(const char *s1);
void					ft_free_list(t_list **list);
int						size_list(t_cmdarg *node);
char					*ft_get_pwd(t_list *env);
bool					is_ambiguous(char *file);
int						ft_open_file(char *file, int flag);
void					handle_input(t_redi_list *input);
void					handle_output(t_redi_list *output);
int						handel_append(t_redi_list *output);
void					handle_heredoc(t_redi_list *input);
void					ft_free_list_heredoc(t_list_heredoc *list);
void					ft_int_list_heredoc(t_list_heredoc *list);

//--builtins
void 					ft_helper(char **key, char **value, char *cmd);
void					ft_handle_append(t_list *dup_key, char **key,
							char **value);
int						ft_handle_plus(char *cmd, char **key, char **value);
int						size_dp(char **c);
t_list					*ft_find_node(t_list *env, char *key);
void					ft_read_line(char *delimiter, int *fd_pipe,
							t_redi_list *heredoc, t_list *env);
void					init_redi_file(t_cmdarg *shell);
void					ft_update_path(t_list *env, char *new_path,
							char *old_path);
int						run_built_in(t_cmdarg *shell, t_list **env);
void					handle_input(t_redi_list *input);
void					handle_output(t_redi_list *output);
int						check_builtin(t_cmdarg *cmdarg_list, t_list **minienv);
char					**handle_quote(char **cmd);
void					ft_cmd_error(char *cmd_name, char *error, int status);
void					ft_free_and_error(char **cmd_path, char **cmd_name,
							char **envp);
void					ft_print_sorted_env(t_list **env);
void					ft_print_export_env(t_list *head);
void					ft_swap_list(t_list *ptr1);
t_list					*ft_copy_list(t_list *env);
void					ft_reset_std(t_cmdarg *shell);
int						ft_open_redi_builtin(char *file, int flag);
void					ft_redi_error(char *file, char *msg, int err);
int						ft_echo(char **cmd, t_cmdarg *env);
int						ft_pwd(t_list **env);
int						ft_unset(char **cmd, t_list **env);
int						ft_exit(char **cmd, t_list **env);
int						ft_cd(char **cmd, t_list **env);
int						ft_env(t_list **env);
int						ft_export(char **cmd, t_list **env);
int						is_builtin(char *cmd);
int						ft_set_env(t_list **env);
void					free_dp(char **cmd);
void					ft_free_char_p(char *s);
int						remove_env_node(t_list **env_list, t_list *node);
void					ft_reset_std(t_cmdarg *shell);
char					*check_full_path(char *p);
void					handle_exec_error(char *cmd_path, char *cmd_name);
void					ft_alloc_dup(t_list *dup_key, char **key, char **value,
							char *cmd);
void					ft_handle_append(t_list *dup_key, char **key,
							char **value);
int						ft_check_name(char *cmd);

// --signals
void					handle_signals(void);

#endif
