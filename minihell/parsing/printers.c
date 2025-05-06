/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:04:33 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 20:31:11 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_printtype(t_token_type type)
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

void	ft_print_env(t_list *minienv)
{
	t_list	*current;

	current = minienv;
	while (current)
	{
		if (current->key && current->value)
		{
			printf("%s", current->key);
			printf("=");
			printf("%s\n", current->value);
		}
		current = current->next;
	}
	printf(RESET"\n"RESET);
}

// void	ft_print_tokenlist(t_token *token_list)
// {
// 	t_token	*tmp;
// 	tmp = token_list;
// 	printf("---------------------------------\n");
// 	printf("Token list:\n");
// 	while (tmp)
// 	{
// 		if (tmp->value)
// 			printf("[%s]-", tmp->value);
// 		tmp = tmp->next;
// 	}
// 	printf("\n");
// 	tmp = token_list;
// 	while (tmp)
// 	{
// 		printf("[%s]-", ft_printtype(tmp->type));
// 		tmp = tmp->next;
// 	}
// 	printf("\n");
// 	tmp = token_list;
// 	while (tmp)
// 	{
// 		if (tmp->addSpace == true)
// 			printf("[addSpace]-");
// 		else
// 			printf("[no addSpace]-");
// 		tmp = tmp->next;
// 	}
// 	printf("\n");
// 	printf("---------------------------------\n");
// }
