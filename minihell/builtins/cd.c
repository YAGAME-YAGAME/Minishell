/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:12:04 by yagame            #+#    #+#             */
/*   Updated: 2025/05/22 01:12:17 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_update_path(t_list *env, char *new_path, char *old_path)
{
	t_list	*old_pwd;
	t_list	*current_pwd;
	char	*tmp;
	char	*cwd;

	ft_free_char_p(new_path);
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
	free(old_path);
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

int	ft_get_target_path(char **cmd, char **path, char *old_path, t_list **env)
{
	char	*tmp;

	tmp = NULL;
	if (cmd[1] == NULL || ft_strcmp(cmd[1], "~") == 0)
	{
		*path = ft_getenv("HOME", *env);
		if (*path == NULL)
			return (free(old_path), write(2, CD_HOME_ERR, 28), 1);
	}
	else if (ft_strcmp(cmd[1], "-") == 0)
	{
		*path = ft_getenv("OLDPWD", *env);
		if (*path == NULL)
			return (free(old_path), write(2, CD_OLDPWD_ERR, 30), 1);
	}
	else
	{
		tmp = ft_handel_tilde(cmd[1], *env);
		if (tmp == NULL)
			return (free(old_path), write(2, CD_HOME_ERR, 28), 1);
		*path = ft_strdup(tmp);
		if (tmp != cmd[1])
			free(tmp);
	}
	return (0);
}

int	ft_cd(char **cmd, t_list **env)
{
	char	*path;
	char	*old_path;

	path = NULL;
	old_path = ft_getenv("PWD", *env);
	if (size_dp(cmd) > 2)
		return (free(old_path), write(2, "minishell :cd: too many arguments\n",
				34), 1);
	if (ft_get_target_path(cmd, &path, old_path, env) != 0)
		return (1);
	if (path == NULL)
		return (free(old_path), write(2, "minishell :cd: HOME not set\n", 28),
			1);
	if (chdir(path) != 0)
		return (free(path), perror(path), 1);
	return (ft_update_path(*env, path, old_path), 0);
}
