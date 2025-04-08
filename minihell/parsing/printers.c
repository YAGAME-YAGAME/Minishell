/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:04:33 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/08 11:06:57 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*printtype(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == INPUT)
		return ("INPUT");
	else if (type == OUTPUT)
		return ("OUTPUT");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	else if (type == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	return ("UNKNOWN");
}

void	print_tokenlist(t_token *token_list)
{
	t_token	*tmp;

	tmp = token_list;
	while (tmp)
	{
		if (tmp->value)
			printf("[%s]: {%s}\n", tmp->value, printtype(tmp->type));
		else
			printf("[NULL]\n");
		tmp = tmp->next;
	}
}

void	ft_print_env(t_list *minienv)
{
	t_list	*current;

	current = minienv;
	while (current)
	{
		if (current->key && current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}
