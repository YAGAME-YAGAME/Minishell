/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:10:23 by yagame            #+#    #+#             */
/*   Updated: 2025/04/26 23:22:39 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	    remove_env_node(t_list **env_list, t_list *node)
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

int    ft_set_env(t_list **env)
{
    char *cwd;
    char cwd_buffer[1024];
    
    if (!*env)
    {
        // Get current working directory
        if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
            cwd = ft_strdup(cwd_buffer);
        else
            cwd = ft_strdup("/Users/yagame");
            
        // Add default environment variables
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
	t_list  **env_ptr;
	t_list  *to_remove;
	int     i;

	if (!cmd)
		return (1);
	
	env_ptr = env;
	i = 1; 
	while (cmd[i])
	{
		tmp = *env_ptr;
		while (tmp)
		{
			if (ft_strcmp(tmp->key, cmd[i]) == 0)
			{
				to_remove = tmp;
				tmp = tmp->next; 
				remove_env_node(env_ptr, to_remove);
				break;
			}
			else
				tmp = tmp->next;
		}
		i++;
	}
	return (1);
}
