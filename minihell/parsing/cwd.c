/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:22:45 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 00:01:44 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
