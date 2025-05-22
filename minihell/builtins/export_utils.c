/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:55:23 by codespace         #+#    #+#             */
/*   Updated: 2025/05/22 01:35:58 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_print_export_env(t_list *head)
{
	while (head)
	{
		if (head && head->key)
		{
			if (head->value == NULL)
				printf("declare -x %s\n", head->key);
			else
			{
				printf("declare -x %s=", head->key);
				if (head->value[0] == '\0')
					printf("\"\"\n");
				else
					printf("\"%s\"\n", head->value);
			}
		}
		head = head->next;
	}
}

void	ft_print_sorted_env(t_list **env)
{
	t_list	*ptr1;
	t_list	*head;
	t_list	*temp;
	int		size_list;

	if (*env == NULL)
		return ;
	ptr1 = ft_copy_list(*env);
	head = ptr1;
	size_list = ft_lstsize(ptr1);
	while (size_list > 0)
	{
		while (ptr1 && ptr1->next)
		{
			ft_swap_list(ptr1);
			ptr1 = ptr1->next;
		}
		ptr1 = head;
		size_list--;
	}
	temp = head;
	ft_print_export_env(temp);
	ft_free_list(&head);
}

void	ft_swap_list(t_list *ptr1)
{
	char	*tmp_key;
	char	*tmp_value;

	tmp_key = NULL;
	tmp_value = NULL;
	if (ft_strcmp(ptr1->key, ptr1->next->key) > 0)
	{
		tmp_key = ptr1->key;
		tmp_value = ptr1->value;
		ptr1->key = ptr1->next->key;
		ptr1->value = ptr1->next->value;
		ptr1->next->key = tmp_key;
		ptr1->next->value = tmp_value;
	}
}

void	ft_alloc_dup(t_list *dup_key, char **key, char **value, char *cmd)
{
	if (dup_key)
	{
		if (ft_strchr(cmd, '=') && dup_key->value)
		{
			free(dup_key->value);
			dup_key->value = *value;
		}
		if (ft_strchr(cmd, '='))
			dup_key->value = *value;
		free(*key);
	}
}

int	ft_check_name(char *cmd)
{
	while (*cmd && *cmd != '+')
		cmd++;
	while (*cmd && *cmd != '=')
		cmd++;
	if (*cmd == '=' && *(cmd - 1) != '+')
		return (0);
	return (1);
}
