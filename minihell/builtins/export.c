/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:19 by yagame            #+#    #+#             */
/*   Updated: 2025/04/18 01:13:27 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int    ft_export(char **cmd, t_list **env)
{
    char *key;
    char *value;

    if(!env || !(*env) || cmd[1] == NULL)
        return (1);
        
    key = ft_substr(cmd[1], 0, ft_strchr(cmd[1], '=') - cmd[1]);
    value = ft_substr(cmd[1], ft_strchr(cmd[1], '=') - cmd[1] + 1,
            ft_strlen(cmd[1]) - (ft_strchr(cmd[1], '=') - cmd[1]));
    if (!key || !value)
        return (-1);
    ft_lstadd_back(env, ft_lstnew(key, value));
    return (1);
}