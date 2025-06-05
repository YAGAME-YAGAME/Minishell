/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:22:45 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Adds a token to the end of a token linked list.
 * Appends a new token to the end of the token list, properly setting
 * the next and previous pointers to maintain the doubly-linked list
 * structure. Handles the case of adding to an empty list.
 *
 * @param token_list: Pointer to the head of the token list
 * @param token: Token to add to the end of the list
 * Side effects: Modifies list structure and token pointers
 */
void	ft_tokadd_back(t_token **token_list, t_token *token)
{
	t_token	*tmp;

	if (!token_list || !token)
		return ;
	if (*token_list == NULL)
	{
		*token_list = token;
		return ;
	}
	tmp = *token_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
	token->prev = tmp;
	token->next = NULL;
}

/*
 * Performs variable expansion in character strings.
 * Processes all variable references ($VAR, $?, $$) in a character string
 * and replaces them with their expanded values. Handles multiple variables
 * in a single string and manages memory allocation for the expanded result.
 *
 * @param value: Pointer to string containing variable references
 * @param minienv: Environment variables list for variable lookup
 * Side effects: Modifies the string pointer, frees/allocates memory
 */
void	ft_expand_var_in_char(char **value, t_list *minienv)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*temp;
	char	*temp2;

	dollar_pos = ft_dollar_pos(*value);
	while (dollar_pos != -1)
	{
		if (ft_condition_inchar(*value, dollar_pos))
		{
			ft_expand_exit_status_inchar(value);
			dollar_pos = ft_dollar_pos(*value);
			continue ;
		}
		expanded = ft_expand_inchar(*value, minienv, dollar_pos);
		temp = ft_strjoin_free(ft_substr(*value, 0, dollar_pos), expanded);
		temp2 = ft_temp2_inchar(*value, dollar_pos);
		*value = ft_strjoin_free(temp, temp2);
		free(temp2);
		dollar_pos = ft_dollar_pos(*value);
	}
}
