/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:52:31 by yagame            #+#    #+#             */
/*   Updated: 2025/05/18 01:48:00 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_list(t_list **list)
{
	t_list	*current;
	t_list	*next;

	if (!list || !*list)
		return ;
	current = *list;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	*list = NULL;
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

char	*ft_join_with_path(char *p)
{
	char	*path;
	char	*tmp;

	tmp = NULL;
	path = getcwd(NULL, 0);
	if (!path)
		return (NULL);
	tmp = ft_strjoin(path, "/");
	if (!tmp)
	{
		free(path);
		return (NULL);
	}
	free(path);
	tmp = my_strjoin(tmp, p);
	if (!tmp)
		return (NULL);
	if (access(tmp, X_OK) == 0)
		return (tmp);
	free(tmp);
	return (NULL);
}

char	*check_exec(char *p, t_list *env)
{
	char	*full_path;
	char	*path;
	char	**path_cmd;
	char	*dir_path;

	if (!p)
		return (NULL);
	full_path = check_full_path(p);
	if (full_path)
		return (full_path);
	dir_path = ft_join_with_path(p);
	if (dir_path)
		return (dir_path);
	free(dir_path);
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
