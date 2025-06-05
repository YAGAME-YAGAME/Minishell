/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:52:31 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 04:28:16 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Frees a linked list of environment variables.
 * Iterates through the list, freeing each node's key, value,
 * and the node itself.
 * Sets the list pointer to NULL after cleanup. Used for memory management
 * of dynamically allocated environment lists.
 *
 * @param list: Pointer to the head of the linked list to free
 * Side effects: Frees all allocated memory, modifies list pointer
 */
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

/*
 * Searches for the PATH environment variable in the environment list.
 * Iterates through the environment linked list to find the node with
 * key "PATH" and returns its value. Used for command path resolution
 * when executing external programs.
 *
 * @param path: Environment variables linked list
 * @return: PATH value string if found, NULL if not found
 */
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

/*
 * Searches for an executable command in PATH directories.
 * Iterates through PATH directories, constructs full paths by joining
 * directory with command name, and tests each for executable access.
 * Returns the first valid executable path found.
 *
 * @param full_path: Pointer to store the found executable path
 * @param path_cmd: Array of PATH directory strings
 * @param p: Command name to search for
 * Side effects: Allocates memory for path construction,
 * modifies full_path pointer
 */
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

/*
 * Attempts to find command in current working directory.
 * Constructs a path by joining the current working directory with
 * the command name and checks if it's executable. Used when command
 * doesn't contain path separators but might be in current directory.
 *
 * @param p: Command name to search for in current directory
 * @return: Full path to executable if found and accessible, NULL otherwise
 * Side effects: Allocates memory for path construction, calls getcwd()
 */
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

/*
 * Resolves command name to full executable path.
 * Main command resolution function that handles different path cases:
 * absolute/relative paths, current directory search, and PATH variable search.
 * Implements the shell's command lookup algorithm.
 *
 * @param p: Command name or path to resolve
 * @param env: Environment variables list for PATH lookup
 * @return: Full path to executable if found, NULL if not found
 * Side effects: Allocates memory for path strings, performs file system access
 */
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
