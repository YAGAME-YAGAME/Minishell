/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:13:19 by yagame            #+#    #+#             */
/*   Updated: 2025/05/03 20:34:31 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_copy_list(t_list *env)
{
	t_list	*new_list;
	t_list	*current;
	char    *key_cp;
	char    *value_cp;
	
	current = env;
	new_list = NULL;
	if(!env)
		return (NULL);
		
	while (current)
	{
		// Safely handle NULL values
		key_cp = current->key ? ft_strdup(current->key) : ft_strdup("");
		value_cp = current->value ? ft_strdup(current->value) : ft_strdup("");
		
		ft_lstadd_back(&new_list, ft_lstnew(key_cp, value_cp));
		current = current->next;
	}
	return new_list;
}

void	ft_swap_list(t_list *ptr1)
{
	char *tmp_key;
	char *tmp_value;

	tmp_key = NULL;
	tmp_value = NULL;
	if (strcmp(ptr1->key, ptr1->next->key) > 0)
	{
		tmp_key = ptr1->key;
		tmp_value = ptr1->value;
		ptr1->key = ptr1->next->key;
		ptr1->value = ptr1->next->value;
		ptr1->next->key = tmp_key;
		ptr1->next->value = tmp_value;
	}
}

void	ft_print_sorted_env(t_list **env)
{
	t_list *ptr1;
	t_list *head;
	t_list *temp;
	int size_list;

	if (*env == NULL)
		return;
	ptr1 = ft_copy_list(*env);
	head = ptr1;
	size_list = ft_lstsize(ptr1);
	while(size_list > 0)
	{
		while (ptr1 && ptr1->next)
		{
			ft_swap_list(ptr1);
			ptr1 = ptr1->next;
		}
		ptr1 = head;
		size_list--;
	}
	while(head)
	{
		if (head && head->key)
			printf("declare -x %s=\"%s\"\n", head->key, head->value ? head->value : "");
		temp = head;
		head = head->next;
		// Free memory for this node
		free(temp->key);
		free(temp->value);
		free(temp);
	}
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

	dup_key = NULL;
	*key = ft_substr(cmd, 0, ft_strchr(cmd, '=') - cmd);
	*value = ft_substr(cmd, ft_strchr(cmd, '=') - cmd + 1,
			ft_strlen(cmd) - (ft_strchr(cmd, '=') - cmd));
	if (!*key)
		return ;
	dup_key = check_dup_env(*key, *env);
	if(dup_key)
	{
		if(dup_key->value)
			free(dup_key->value);
		dup_key->value = *value;
	}
	else
		ft_lstadd_back(env, ft_lstnew(*key, *value));
}

int	check_error(char *cmd)
{
	if (*cmd == '\0')
		return (write(2, "export: `' : not a valid identifier\n", 36), 1);
	if (*cmd == '=')
		return (write(2, "export: `= : not a valid identifier\n", 36), 1);
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
	char *key;
	char *value;
	int ret = 0;

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
