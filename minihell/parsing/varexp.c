/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varexp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:34:36 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/18 20:51:16 by otzarwal         ###   ########.fr       */
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
	char	*leak;

	dollar_pos = ft_dollar_pos((*token)->value);
	if ((*token)->value[dollar_pos + 1] == '?')
		expanded = ft_itoa(g_exit_status);
	else if ((*token)->value[dollar_pos + 1] == '$')
		expanded = ft_itoa(getpid());
	else
		expanded = ft_strdup("");
	leak = (*token)->value;
	temp = ft_strjoin_free(ft_substr((*token)->value, 0, dollar_pos), expanded);
	temp2 = ft_substr((*token)->value, dollar_pos + 2,
			ft_strlen((*token)->value));
	(*token)->value = ft_strjoin_free(temp, temp2);
	free(expanded);
	free(leak);
	(*token)->variable = true;
	free(temp2);
}

void	ft_expand_variables(t_token **token, t_list *minienv)
{
	ssize_t	dpos;
	char	*exp;
	char	*temp;
	char	*temp2;

	if ((*token)->type == SINGLE_QUOTE)
		return ;
	dpos = ft_dollar_pos((*token)->value);
	while (dpos != -1)
	{
		if (ft_condition(token, dpos))
		{
			ft_expand_exit_status(token);
			dpos = ft_dollar_pos((*token)->value);
			continue ;
		}
		exp = ft_expand(token, minienv, dpos);
		temp = ft_strjoin_free(ft_substr((*token)->value, 0, dpos), exp);
		temp2 = ft_temp2(token, dpos);
		free((*token)->value);
		(*token)->value = ft_strjoin_free(temp, temp2);
		free(temp2);
		(*token)->variable = true;
		free(exp);
		dpos = ft_dollar_pos((*token)->value);
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
			return (ft_strdup(tmp->value));
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
