#include "../../includes/Minishell.h"

void	ft_init_minishell(t_minishell *minishell, char **envp)
{
	minishell->env = NULL;
	minishell->syntax_error = false;
	minishell->pipes_nbr = 0;
	minishell->env = ft_strdup_2d(envp);
	minishell->tree = NULL;
}

void	ft_eof(t_minishell *minishell)
{
	printf(RED"\n[EOF]\n"RESET);
	free(minishell->cwd);
	ft_free_2d(minishell->env);
	exit(0);
}

void	ft_free_init(t_minishell *minishell)
{
	free(minishell->cwd);
	free(minishell->input);
}
