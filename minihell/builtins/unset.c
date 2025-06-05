/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:10:23 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Removes a specific node from the environment variables linked list.
 * Handles proper unlinking of doubly-linked list node by updating
 * previous and next node pointers. Frees all allocated memory including
 * key, value strings and the node structure itself.
 *
 * @param env_list: Pointer to the head of environment list
 * @param node: List node to remove and free
 * @return: Always returns 1 (success indicator)
 * Side effects: Modifies linked list structure, frees memory
 */
int	remove_env_node(t_list **env_list, t_list *node)
{
	if (!node)
		return (1);
	if (node->prev)
		node->prev->next = node->next;
	else
		*env_list = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	return (1);
}

/*
 * Initializes a minimal environment when none exists.
 * Creates essential shell environment variables including PWD (current working
 * directory), OLDPWD, SHLVL (shell level), and _ (last command). Used when
 * shell starts with empty environment or as fallback initialization.
 *
 * @param env: Pointer to environment variables linked list (should be empty)
 * @return: 1 if environment was initialized, 0 if environment already exists
 * Side effects: Allocates memory for environment nodes, calls getcwd()
 */
int	ft_set_env(t_list **env)
{
	char	*cwd;
	char	cwd_buffer[1024];

	if (!*env || !env)
	{
		if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
			cwd = ft_strdup(cwd_buffer);
		else
			cwd = ft_strdup("/Users/yagame");
		ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"), NULL));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), cwd));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("SHLVL"), ft_strdup("1")));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("_"),
				ft_strdup("/usr/bin/env")));
		return (1);
	}
	return (0);
}

/*
 * Implements the unset builtin command functionality.
 * Removes specified environment variables from the shell's environment.
 * Iterates through all provided variable names and removes matching entries
 * from the environment list. Protects special variables like '_' from removal.
 *
 * @param cmd: Array of variable names to unset (cmd[0] is "unset")
 * @param env: Pointer to environment variables linked list
 * @return: Always returns 1 (success indicator)
 * Side effects: Modifies environment list by removing specified variables
 */
int	ft_unset(char **cmd, t_list **env)
{
	t_list	*tmp;
	t_list	**env_ptr;
	t_list	*to_remove;

	if (!cmd || !*cmd || !env || !*env)
		return (1);
	env_ptr = env;
	while (*cmd)
	{
		tmp = *env_ptr;
		while (tmp)
		{
			if ((*cmd[0] != '_') && ft_strcmp(tmp->key, *cmd) == 0)
			{
				to_remove = tmp;
				tmp = tmp->next;
				remove_env_node(env_ptr, to_remove);
				break ;
			}
			else
				tmp = tmp->next;
		}
		cmd++;
	}
	return (1);
}
