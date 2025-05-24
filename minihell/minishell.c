/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/24 12:29:39 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit_status = 0;

int	ft_toksize(t_token *lst)
{
	t_token	*p;
	int		len;

	if (lst == NULL)
		return (0);
	p = lst;
	len = 0;
	while (p != NULL)
	{
		p = p->next;
		len++;
	}
	return (len);
}

int	ft_check_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	if (ft_toksize(token_list) == 1 && ft_isredi(current))
		return (printf(RED SYNTAX_ERROR RESET), g_exit_status = 258, -1);
	while (current)
	{
		if (ft_pipeerrors(current))
			return (printf(RED PIPE_ERROR RESET), g_exit_status = 258, -1);
		if (ft_redierrors(current))
			return (printf(RED SYNTAX_ERROR RESET), g_exit_status = 258, -1);
		current = current->next;
	}
	return (0);
}

t_cmdarg	*ft_parser(t_token *token_list, t_list *minienv)
{
	t_cmdarg	*cmdarg_list;
	t_cmdarg	*node;
	t_token		*tmp;

	if (!token_list)
		return (NULL);
	cmdarg_list = NULL;
	tmp = token_list;
	while (tmp)
	{
		if (!(tmp->type == WORD && tmp->prev && tmp->prev->type == HEREDOC))
			ft_expand_variables(&tmp, minienv);
		tmp = tmp->next;
	}
	token_list->current = token_list;
	node = ft_get_next_node(token_list);
	while (node)
	{
		if (node)
			ft_nodeadd_back(&cmdarg_list, ft_newnode(node));
		freeall(node->cmd, node->cmdsize);
		free(node);
		node = ft_get_next_node(token_list);
	}
	return (cmdarg_list);
}

void	minishell(char *input, t_list **minienv)
{
	t_token		*token_list;
	t_cmdarg	*cmdarg_list;

	if (input == NULL)
		ft_cmd_error(NULL, RED "[EOF]\n" RESET, 0);
	if (input[0] == '\0')
		return ;
	add_history(input);
	token_list = ft_strtok(input, *minienv);
	// ft_print_tokenlist(token_list);
	if (ft_check_syntax(token_list) == -1)
		return (ft_free_tokenlist(token_list));
	cmdarg_list = ft_parser(token_list, *minienv);
	// ft_printcmd_list(cmdarg_list);
	if (!check_here_doc(cmdarg_list, *minienv))
		return (ft_cleaner(token_list, cmdarg_list));
	if (check_builtin(cmdarg_list, minienv) == 1)
		return (ft_cleaner(token_list, cmdarg_list));
	if (!execution(cmdarg_list, *minienv))
		return (ft_cleaner(token_list, cmdarg_list));
	ft_cleaner(token_list, cmdarg_list);
}

void	ll(void)
{
	system("leaks -q minishell");
}
int	main(int ac, char **av, char **env)
{
	t_list	*minienv;
	char	*input;

	// char	*cwd;
	handle_signals();
	// atexit(ll);
	(void)av;
	if (ac != 1)
		return (printf(YELLOW "\nError: No arguments expected\n" RESET), 1);
	else
	{
		minienv = ft_envinit(env);
		while (1)
		{
			// cwd = ft_getcwd(minienv);
			input = readline(GREEN "minishell-1.0$ " RESET);
			minishell(input, &minienv);
			free(input);
			// free(cwd);
		}
	}
	return (0);
}
