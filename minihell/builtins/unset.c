/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:10:23 by yagame            #+#    #+#             */
/*   Updated: 2025/05/03 23:54:55 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	ft_free_list(&node);
	return (1);
}

int	ft_set_env(t_list **env)
{
	char	*cwd;
	char	cwd_buffer[1024];

	if (!*env || !env)
	{
		if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
			cwd = ft_strdup(cwd_buffer);
		else
			cwd = ft_strdup("/Users/yagame"); // FIXME:
		ft_lstadd_back(env, ft_lstnew(ft_strdup("OLDPWD"), NULL));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("PWD"), cwd));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("SHLVL"), ft_strdup("1")));
		ft_lstadd_back(env, ft_lstnew(ft_strdup("_"), ft_strdup("/usr/bin/env")));
		return (1);
	}
	return (0);
}


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
				break;
			}
			else
				tmp = tmp->next;
		}
		cmd++;
	}
	return (1);
}
