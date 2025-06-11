/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:12:04 by yagame            #+#    #+#             */
/*   Updated: 2025/06/09 02:26:01 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Updates PWD and OLDPWD environment variables after directory change.
 * Sets OLDPWD to the previous working directory and PWD to the current
 * working directory. Handles cases where environment variables may not exist.
 *
 * @param env: Environment variables linked list
 * @param new_path: Path string to free (from target path resolution)
 * @param old_path: Previous working directory path
 * Side effects: Modifies environment variables, allocates/frees memory,
 * calls getcwd
 */
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

/*
 * Handles tilde expansion for cd command paths.
 * Expands paths beginning with '~' by replacing the tilde with the
 * HOME environment variable value. Returns the original command if
 * no tilde expansion is needed.
 *
 * @param cmd: Command path that may contain tilde
 * @param env: Environment variables list for HOME lookup
 * @return: Expanded path string or original command, NULL if HOME not set
 * Side effects: May allocate memory for expanded path
 */
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

/*
 * Determines the target directory path for the cd command.
 * Handles various cd cases: no argument (HOME), '-' (OLDPWD), tilde expansion,
 * and regular paths. Validates that required environment variables exist
 * and handles error cases appropriately.
 *
 * @param cmd: Command array containing cd and its arguments
 * @param path: Pointer to store the resolved target path
 * @param old_path: Current working directory path for error cleanup
 * @param env: Pointer to environment variables list
 * @return: 0 on success, 1 on error
 * Side effects: Allocates memory for path, may write error messages,
 * frees old_path on error
 */
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

/*
 * Implements the cd builtin command.
 * Changes the current working directory to the specified path, handling
 * special cases like HOME directory, previous directory (-),
 * and tilde expansion.
 * Updates PWD and OLDPWD environment variables and validates arguments.
 *
 * @param cmd: Command array where cmd[0] is "cd" and
 * cmd[1] is target directory
 * @param env: Pointer to environment variables list
 * @return: 0 on success, 1 on error
 * Side effects: Changes working directory, modifies environment variables,
 * may write errors
 */
int	ft_cd(char **cmd, t_list **env)
{
	char	*path;
	char	*old_path;
	int		is_dash;

	path = NULL;
	old_path = ft_getenv("PWD", *env);
	is_dash = (cmd[1] != NULL && ft_strcmp(cmd[1], "-") == 0);
	if (size_dp(cmd) > 2)
		return (free(old_path), write(2, "minishell :cd: too many arguments\n",
				34), 1);
	if (ft_get_target_path(cmd, &path, old_path, env) != 0)
		return (free(path), 1);
	if (path == NULL)
		return (free(old_path), write(2, "minishell :cd: HOME not set\n", 28),
			1);
	if (chdir(path) != 0)
		return (perror(path), free(path), free(old_path), 1);
	if (is_dash)
	{
		write(1, path, ft_strlen(path));
		write(1, "\n", 1);
	}
	ft_update_path(*env, path, old_path);
	return (0);
}
