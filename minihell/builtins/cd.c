/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:12:04 by yagame            #+#    #+#             */
/*   Updated: 2025/04/18 01:12:51 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../minishell.h"


 t_list  *ft_find_node(t_list *env, char *key)
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
 void    ft_update_path(t_list *env, char *new_path)
 {
     t_list *old_pwd;
     t_list *pwd;
     char *hold;
     // char *tmp;
     
     // tmp = NULL;
     hold = NULL;
     pwd = ft_find_node(env, "PWD");
     old_pwd = ft_find_node(env, "OLDPWD");
     if(old_pwd)
         old_pwd->value = pwd->value;
     else
     {
         old_pwd = ft_lstnew("PWD", new_path);
         ft_lstadd_back(&env, pwd);
     }
     if(pwd)
     {
         // tmp = pwd->value;
         pwd->value = new_path;
         // free(tmp);
     }
     else
     {
         pwd = ft_lstnew("PWD", hold);
         ft_lstadd_back(&env, old_pwd);
     }
 }

 
 int    ft_cd(char **cmd, t_list *env)
{
    char *path;
    
    path = NULL;
    if (cmd[1] == NULL || ft_strcmp(cmd[1], "~") == 0)
    {
        path = getenv("HOME");
        if (path == NULL)
        {
            write(2, "cd: HOME not set\n", 17);
            return (-1);
        }
    }
    else if (ft_strcmp(cmd[1], "-") == 0)
    {
        path = getenv("OLDPWD");
        if (path == NULL)
        {
            write(2, "cd: OLDPWD not set\n", 19);
            return (-1);
        }
    }
    else
        path = cmd[1];
    if (chdir(path) != 0)
    {
        perror("cd");
        return (-1);
    }
    path = cmd[1];
    ft_update_path(env, path);
    return (1);
}