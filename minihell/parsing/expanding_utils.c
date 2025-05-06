/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:48:03 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 20:33:19 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_condition(t_token **token, ssize_t dollar_pos)
{
	return ((*token)->value[dollar_pos + 1] == '$'
		|| (*token)->value[dollar_pos + 1] == '?'
		|| ft_isdigit((*token)->value[dollar_pos + 1]));
}

char	*ft_expand(t_token **token, t_list *minienv, ssize_t dollar_pos)
{
	char	*var;
	char	*expanded;

	var = ft_substr((*token)->value, dollar_pos + 1,
			ft_get_varlen((*token)->value + dollar_pos + 1));
	expanded = ft_getvar(var, minienv);
	free(var);
	return (expanded);
}

char	*ft_temp2(t_token **token, ssize_t dollar_pos)
{
	return (ft_substr((*token)->value,
			dollar_pos + ft_get_varlen((*token)->value + dollar_pos + 1) + 1,
			ft_strlen((*token)->value)));
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}
