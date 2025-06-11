/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:19 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 13:35:42 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Creates a complete copy of an environment variable linked list.
 * Duplicates all nodes with their key-val pairs to create an independent cpy
 * that can be modified without affecting the original environment list.
 * Used primarily for sorting environment variables for display.
 *
 * @param env: Source environment linked list to copy
 * @return: New linked list copy, NULL if source is NULL or memory alloc fails
 * Side effects: Allocates memory for new list nodes and strings
 */

/*
 * Searches for an existing environment variable by key name.
 * Performs case-sensitive string comparison to find matching environment
 * variable in the linked list. Used to check for duplicate keys before
 * adding new variables or when updating existing ones.
 *
 * @param key: Environment variable name to search for
 * @param env: Environment variables linked list to search in
 * @return: Pointer to matching list node, NULL if not found
 */
t_list	*check_dup_env(char *key, t_list *env)
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

/*
 * Allocates and processes key-value pairs for export command arguments.
 * Handles parsing of export arguments including append operations (VAR+=value)
 * and regular assignments (VAR=value). Manages duplicate key detection and
 * either updates existing variables or creates new ones.
 *
 * @param cmd: Export command argument string
 * (e.g., "VAR=value" or "VAR+=append")
 * @param key: Pointer to store extracted variable key
 * @param value: Pointer to store extracted variable value
 * @param env: Environment variables linked list to modify
 * Side effects: May modify environment list, allocates memory for key/value
 */
void	ft_alloc_key_value(char *cmd, char **key, char **value, t_list **env)
{
	t_list	*dup_key;
	int		is_append;

	dup_key = NULL;
	is_append = 0;
	if (ft_strchr(cmd, '='))
	{
		is_append = ft_handle_plus(cmd, key, value);
		if (is_append == -1)
			return ;
	}
	else
		ft_helper(key, value, cmd);
	dup_key = check_dup_env(*key, *env);
	if (dup_key)
	{
		if (is_append && dup_key->value)
			ft_handle_append(dup_key, key, value);
		else
			ft_alloc_dup(dup_key, key, value, cmd);
	}
	else
		ft_lstadd_back(env, ft_lstnew(*key, *value));
}

/*
 * Validates shell variable identifier names according to shell naming rules.
 * Checks that variable names start with letter or underscore, contain only
 * alphanumeric characters and underscores,
 * and handles append operator validation.
 * Ensures compliance with POSIX shell variable naming conventions.
 *
 * @param str: Variable identifier string to validate
 * @return: 1 if valid identifier, 0 if invalid
 */
static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '+')
	{
		if (!str[i + 1] || str[i + 1] != '=')
			return (0);
	}
	return (1);
}

/*
 * Validates export command arguments and reports syntax errors.
 * Checks for proper variable identifier format, handles special cases like
 * empty arguments and invalid characters. Extracts variable name portion
 * for validation regardless of assignment or append operations.
 *
 * @param cmd: Export command argument to validate
 * @return: 0 if valid, 1 if error (with error message printed)
 * Side effects: Writes error messages to stderr, allocates temporary memory
 */
int	check_error(char *cmd)
{
	char	*equals_pos;
	char	*name_end;
	char	*tmp;

	if (!cmd || !*cmd)
		return (write(2, "export: ` : not a valid identifier\n", 36), 1);
	if (*cmd == '=')
		return (print_invalid_identifier("="));
	equals_pos = ft_strchr(cmd, '=');
	if (!equals_pos)
		name_end = cmd + ft_strlen(cmd);
	else
		name_end = equals_pos;
	if (ft_strchr(cmd, '+'))
		name_end = ft_strchr(cmd, '+');
	tmp = ft_substr(cmd, 0, name_end - cmd);
	if (!tmp)
		return (1);
	if (!is_valid_identifier(tmp))
	{
		free(tmp);
		return (print_invalid_identifier(cmd));
	}
	free(tmp);
	return (0);
}

/*
 * Implements the export builtin command functionality.
 * Handles environment variable creation, modification, and display operations.
 * When called without arguments,
 * displays all environment variables in sorted order.
 * With arguments, processes each as a variable assignment or declaration.
 *
 * @param cmd: Array of command arguments (cmd[0] is "export")
 * @param env: Pointer to environment variables linked list
 * @return: 0 on success, 1 on error
 * Side effects: May modify environment list, prints to stdout/stderr
 */
int	ft_export(char **cmd, t_list **env)
{
	char	*key;
	char	*value;
	int		ret;

	ret = 0;
	key = NULL;
	value = NULL;
	if (!env || !*env)
		return (1);
	if (!cmd[1])
		ft_print_sorted_env(env);
	cmd++;
	while (*cmd)
	{
		if (check_error(*cmd) != 0)
			ret = 1;
		else
			ft_alloc_key_value(*cmd, &key, &value, env);
		cmd++;
	}
	return (ret);
}
