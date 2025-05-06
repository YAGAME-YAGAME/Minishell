/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:18:40 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/04 19:29:07 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_isredi(t_token *token)
{
	return (token->type == INPUT || token->type == OUTPUT
		|| token->type == HEREDOC || token->type == APPEND);
}

t_redi_list	*ft_redinew(t_token *token, bool expand)
{
	t_redi_list	*new;

	new = malloc(sizeof(t_redi_list));
	if (!new)
		return (NULL);
	new->type = token->type;
	new->file = token->next->value;
	new->expand = expand;
	new->variable = token->next->variable;
	new->next = NULL;
	return (new);
}

void	ft_rediradd(t_redi_list **redi, t_redi_list *new)
{
	t_redi_list	*tmp;

	if (!*redi)
	{
		*redi = new;
		return ;
	}
	tmp = *redi;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_printredi(t_redi_list *redi)
{
	t_redi_list	*tmp;

	tmp = redi;
	while (tmp)
	{
		printf("type: {%s}, file: [%s]:%d\n", ft_printtype(tmp->type),
			tmp->file, tmp->variable);
		tmp = tmp->next;
	}
}
