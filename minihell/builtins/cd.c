/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:12:04 by yagame            #+#    #+#             */
/*   Updated: 2025/05/15 21:04:27 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	size_dp(char **c)
{
	int	i;

	i = 0;
	while (*c)
	{
		c++;
		i++;
	}
	return (i);
}

t_list	*ft_find_node(t_list *env, char *key)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_update_path(t_list *env, char *new_path, char *old_path)
{
	t_list	*old_pwd;
	t_list	*current_pwd;
	char	*tmp;
	char	*cwd;

	if(new_path)
		free(new_path);
	tmp = NULL;
	cwd = getcwd(NULL, 0);
	old_pwd = ft_find_node(env, "OLDPWD");
	current_pwd = ft_find_node(env, "PWD");
	if (old_pwd)
	{
		if (old_pwd->value)
			free(old_pwd->value);
		old_pwd->value = ft_strdup(old_path);
	}
	if (current_pwd)
	{
		tmp = current_pwd->value;
		current_pwd->value = cwd;
		free(tmp);
	}
	else
		free(cwd);
}

char	*ft_handel_tilde(char *cmd, t_list *env)
{
	char	*home;
	char	*tmp;

	tmp = NULL;
	if (cmd[0] == '~')
	{
		home = ft_getenv("HOME", env);
		if (home == NULL)
			return (NULL);
		tmp = my_strjoin(home, cmd + 1);
	}
	else
		tmp = cmd;
	return (tmp);
}

int	ft_cd(char **cmd, t_list **env)
{
	char	*path;
	char	*old_path;

	path = NULL;
	old_path = ft_getenv("PWD", *env);
	if (size_dp(cmd) > 2)
		return (free(cmd), write(2, "minishell :cd: too many arguments\n", 34),
			1);
	if (cmd[1] == NULL || ft_strcmp(cmd[1], "~") == 0)
	{
		path = ft_getenv("HOME", *env);
		if (path == NULL)
			return (write(2, "minishell :cd: HOME not set\n", 28), 1);
	}
	else if (ft_strcmp(cmd[1], "-") == 0)
	{
		path = ft_getenv("OLDPWD", *env);
		if (path == NULL)
			return (write(2, "minishell :cd: OLDPWD not set\n", 30), 1);
	}
	else
		path = ft_handel_tilde(cmd[1], *env);
	if (chdir(path) != 0)
		return (perror(path), 1);
	return (ft_update_path(*env, path, old_path), 0);
}
