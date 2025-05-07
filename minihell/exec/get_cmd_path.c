/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:52:31 by yagame            #+#    #+#             */
/*   Updated: 2025/05/07 16:46:58 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_list(t_list **list)
{
	if (!list || !*list)
		return ;
	if ((*list)->key)
		free((*list)->key);
	if ((*list)->value)
		free((*list)->value);
	free(*list);
}

char	*find_path(t_list *path)
{
	t_list	*tmp;

	tmp = path;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "PATH", 4))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_get_path(char **full_path, char **path_cmd, char *p)
{
	char	*tmp;

	tmp = NULL;
	*full_path = NULL;
	while (*path_cmd)
	{
		*full_path = ft_strjoin(*path_cmd, "/");
		if (!*full_path)
			return ;
		tmp = *full_path;
		*full_path = ft_strjoin(*full_path, p);
		free(tmp);
		tmp = NULL;
		if (!*full_path)
			return ;
		if (access(*full_path, X_OK) == 0)
			return ;
		free(*full_path);
		*full_path = NULL;
		path_cmd++;
	}
}

char	*check_exec(char *p, t_list *env)
{
	char	*full_path;
	char	*path;
	char	**path_cmd;

	if (!p)
		return (NULL);
	full_path = NULL;
	if (*p == '/' || *p == '.')
	{
		if (access(p, X_OK) == 0)
			return (ft_strdup(p));
		else
			return (NULL);
	}
	path = find_path(env);
	if (!path)
		return (NULL);
	path_cmd = parsing_split(path, ':');
	if (!path_cmd)
		return (NULL);
	ft_get_path(&full_path, path_cmd, p);
	free_dp(path_cmd);
	if (!full_path)
		return (NULL);
	return (full_path);
}
