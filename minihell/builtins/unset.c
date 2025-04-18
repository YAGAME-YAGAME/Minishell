/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:10:23 by yagame            #+#    #+#             */
/*   Updated: 2025/04/18 01:10:34 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int    ft_unset(char **cmd, t_list *env)
{
    t_list *tmp;
    
    tmp = NULL;
    if(cmd[1] == NULL || !env)
        return (1);
    tmp = env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, cmd[1]) == 0)
        {
            if (tmp->next)
                tmp->prev->next = tmp->next;
            ft_free_list(&tmp);
            return (1);
        }
        tmp = tmp->next;
    }
    return (1);
}
