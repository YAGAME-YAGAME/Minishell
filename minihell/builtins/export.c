/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:19 by yagame            #+#    #+#             */
/*   Updated: 2025/05/23 04:24:24 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_copy_list(t_list *env)
{
	t_list	*new_list;
	t_list	*current;
	char	*key_cp;
	char	*value_cp;

	current = env;
	new_list = NULL;
	if (!env)
		return (NULL);
	while (current)
	{
		if (current->key)
			key_cp = ft_strdup(current->key);
		else
			key_cp = ft_strdup("");
		if (current->value)
			value_cp = ft_strdup(current->value);
		else
			value_cp = NULL;
		ft_lstadd_back(&new_list, ft_lstnew(key_cp, value_cp));
		current = current->next;
	}
	return (new_list);
}

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

int	check_error(char *cmd)
{
	if (!cmd || !*cmd)
		return (write(2, "export: ` : not a valid identifier\n", 36), 1);
	if (*cmd == '\0')
		return (write(2, "export: `' : not a valid identifier\n", 36), 1);
	if (*cmd == '=')
		return (write(2, "export: `= : not a valid identifier\n", 36), 1);
	if (!ft_strcmp(cmd, "$"))
		return (write(2, "export: `$ : not a valid identifier\n", 36), 1);
	if (cmd[0] == '+' || ft_check_name(cmd) == 0)
		return (write(2, "export: `+ : not a valid identifier\n", 36), 1);
	if (ft_isdigit(*cmd))
	{
		write(2, "export: `", 9);
		write(2, cmd, ft_strlen(cmd));
		write(2, "' : not a valid identifier\n", 27);
		return (1);
	}
	return (0);
}

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
