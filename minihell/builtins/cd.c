/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:12:04 by yagame            #+#    #+#             */
/*   Updated: 2025/05/03 17:58:41 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int     size_dp(char **c)
{
	int i;

	i = 0;
	while(*c)
	{
		c++;
		i++;
	}
	return (i);
}

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
void    ft_update_path(t_list *env, char *new_path, char *old_path)
{
	t_list *old_pwd;
	t_list  *current_pwd;
	// char *hold;
	char *tmp;

	tmp = NULL;
	// hold = NULL;

	new_path = getcwd(NULL, 0);
	old_pwd = ft_find_node(env, "OLDPWD");
	current_pwd = ft_find_node(env, "PWD");
	if(old_pwd)
		old_pwd->value = ft_strdup(old_path);

	if(current_pwd)
	{
		tmp = current_pwd->value;
		current_pwd->value = new_path;
		free(tmp);
	}
	// else
	// {
	//     pwd = ft_lstnew("PWD", hold);
	//     ft_lstadd_back(&env, pwd);
	// }
}



int    ft_cd(char **cmd, t_list **env)
{
	char *path;
	char *old_path;

	path = NULL;
	old_path = ft_getenv("PWD", *env);
	if(size_dp(cmd) > 2)
			return (free(cmd), write(2, "cd : too many arguments\n", 24), 1);
	if (cmd[1] == NULL || ft_strcmp(cmd[1], "~") == 0)
	{
		path = ft_getenv("HOME", *env);
		if (path == NULL)
			return (free_dp(cmd), write(2, "cd: HOME not set\n", 17), 1);
	}
	else if (ft_strcmp(cmd[1], "-") == 0)
	{
		path = ft_getenv("OLDPWD", *env);
		if (path == NULL)
			return (free_dp(cmd), write(2, "cd: OLDPWD not set\n", 19), 1);
			
	}
	else
	   path = cmd[1];
	if (chdir(path) != 0)
	   return (perror(path), 1);
	ft_update_path(*env, path, old_path);
	return (1);
}
