/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:55:23 by codespace         #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Prints environment variables in export format.
 * Displays each environment variable in the "declare -x" format used by
 * the export builtin. Handles variables with null values by omitting the
 * assignment, and properly quotes values containing spaces or special characters.
 *
 * @param head: Environment variables linked list to print
 * Side effects: Prints formatted environment variables to stdout
 */
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

/*
 * Prints environment variables in sorted order for export display.
 * Creates a copy of the environment list, sorts it alphabetically by key
 * using bubble sort, then prints in export format. Used when export
 * command is called without arguments to show all variables.
 *
 * @param env: Pointer to environment variables linked list
 * Side effects: Prints sorted environment to stdout, allocates/frees temporary list
 */
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

/*
 * Swaps two adjacent nodes in environment list during sorting.
 * Compares keys alphabetically and swaps both key and value pointers
 * if the first key is lexicographically greater than the second.
 * Part of the bubble sort implementation for environment display.
 *
 * @param ptr1: First node in the pair to potentially swap
 * Side effects: May swap key and value pointers between adjacent nodes
 */
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

/*
 * Handles memory allocation when updating existing environment variables.
 * Manages memory for duplicate keys in export operations, properly freeing
 * old values and setting new ones. Handles both assignment and declaration
 * cases based on whether the command contains an '=' character.
 *
 * @param dup_key: Existing environment node to update
 * @param key: Pointer to key string (will be freed)
 * @param value: Pointer to value string (may be used or freed)
 * @param cmd: Original command string to check for assignment
 * Side effects: Modifies environment node, frees memory
 */
void	ft_alloc_dup(t_list *dup_key, char **key, char **value, char *cmd)
{
	if (!dup_key || !key || !value)
		return ;
	if (ft_strchr(cmd, '='))
	{
		if (dup_key->value)
			free(dup_key->value);
		dup_key->value = *value;
		*value = NULL;
	}
	else if (*value)
	{
		free(*value);
		*value = NULL;
	}
	if (*key)
	{
		free(*key);
		*key = NULL;
	}
}
