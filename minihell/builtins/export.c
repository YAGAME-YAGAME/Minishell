/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:19 by yagame            #+#    #+#             */
/*   Updated: 2025/04/20 02:38:12 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_print_sorted_env(t_list **env)
{
    t_list *tmp;
    t_list *head;

    head = *env;
    tmp = head;
    while (tmp)
    {
        if (tmp->key)
        {
            printf("%s", tmp->key);
            printf("=%s\n", tmp->value);
        }
        tmp = tmp->next;
    }
}

t_list    *check_dup_env(char *key, t_list *env)
{
    t_list *tmp;

    tmp = env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
            return (tmp);
        tmp = tmp->next;
    }
    return (NULL);
}

void    ft_alloc_key_value(char *cmd, char **key, char **value)
{
    *key = NULL;
    *value = NULL;

    if (ft_strchr(cmd, '=') == NULL)
        return ;
    *key = ft_substr(cmd, 0, ft_strchr(cmd, '=') - cmd);
    *value = ft_substr(cmd, ft_strchr(cmd, '=') - cmd + 1,
            ft_strlen(cmd) - (ft_strchr(cmd, '=') - cmd));
}
int    ft_export(char **cmd, t_list **env)
{
    char *key;
    char *value;
    t_list *dup_key;

    dup_key = NULL;
    if(!env || !(*env))
        return (free_dp(cmd), 1);
    if(cmd[1] == NULL)
        return (ft_print_sorted_env(env), 1);
    if (cmd[1][0] == '\0')
        return(free_dp(cmd),write(1, "export: `' : not a valid identifier\n", 35), 1);
    if (cmd[1][0] == '=')
        return(free_dp(cmd),write(1, "export: `= : not a valid identifier\n", 35), 1);
    if (ft_strchr(cmd[1], '=') == NULL) 
        return (free_dp(cmd), 1);
    ft_alloc_key_value(cmd[1], &key, &value);  
    if (!key || !value)
        return (1);
    dup_key = check_dup_env(key, *env);
    if(dup_key)
    {
        if(dup_key->value)
            free(dup_key->value);
        dup_key->value = ft_strtrim(value, "\"\'");
    }
    else
        ft_lstadd_back(env, ft_lstnew(ft_strtrim(key, "\'\""), ft_strtrim(value, "\"\'")));
    return (1);
}
