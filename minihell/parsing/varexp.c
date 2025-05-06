/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varexp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:34:36 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 20:34:33 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	ft_expand_exit_status(t_token **token)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*temp;
	char	*temp2;

	dollar_pos = ft_dollar_pos((*token)->value);
	if ((*token)->value[dollar_pos + 1] == '?')
		expanded = ft_itoa(g_exit_status);
	else if ((*token)->value[dollar_pos + 1] == '$')
		expanded = ft_itoa(getpid());
	else
		expanded = ft_strdup("");
	temp = ft_strjoin_free(ft_substr((*token)->value, 0, dollar_pos), expanded);
	temp2 = ft_substr((*token)->value,
			dollar_pos + 2,
			ft_strlen((*token)->value));
	(*token)->value = ft_strjoin_free(temp, temp2);
	free(temp2);
}

void	ft_expand_variables(t_token **token, t_list *minienv)
{
	ssize_t	dollar_pos;
	char	*expanded;
	char	*temp;
	char	*temp2;

	if ((*token)->type == SINGLE_QUOTE)
		return ;
	dollar_pos = ft_dollar_pos((*token)->value);
	while (dollar_pos != -1)
	{
		if (ft_condition(token, dollar_pos))
		{
			ft_expand_exit_status(token);
			dollar_pos = ft_dollar_pos((*token)->value);
			continue ;
		}
		expanded = ft_expand(token, minienv, dollar_pos);
		temp = ft_strjoin_free(ft_substr((*token)->value, 0, dollar_pos),
				expanded);
		temp2 = ft_temp2(token, dollar_pos);
		(*token)->value = ft_strjoin_free(temp, temp2);
		(*token)->variable = true;
		free(temp2);
		dollar_pos = ft_dollar_pos((*token)->value);
	}
}

char	*ft_getenv(char *var, t_list *minienv)
{
	t_list	*tmp;

	tmp = minienv;
	if (tmp == NULL)
		return (NULL);
	while (tmp)
	{
		if (ft_strcmp(var, tmp->key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*ft_getvar(char *var, t_list *minienv)
{
	char	*value;

	value = ft_getenv(var, minienv);
	if (!value)
	{
		value = ft_strdup("");
		return (value);
	}
	return (value);
}
