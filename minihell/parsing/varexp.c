/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varexp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:34:36 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/03 18:04:27 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_ExpandVarInChar(char **value, t_list *minienv)
{
	ssize_t	dollar_pos;
	char	*var;
	char	*expanded;
	char	*temp;
	char	*temp2;

	dollar_pos = ft_dollar_pos(*value);
	while (dollar_pos != -1)
	{
		var = ft_substr(*value, dollar_pos + 1,
				ft_get_var_length(*value + dollar_pos + 1));
		expanded = ft_getvar(var, minienv);
		free(var);
		temp = ft_strjoin_free(ft_substr(*value, 0, dollar_pos), expanded);
		temp2 = ft_substr(*value,
				dollar_pos + ft_get_var_length(*value + dollar_pos + 1) + 1,
				ft_strlen(*value));
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
	while (dollar_pos != -1)
	{
		expanded = ft_itoa(g_exit_status);
		temp = ft_strjoin_free(ft_substr((*token)->value, 0, dollar_pos), expanded);
		temp2 = ft_substr((*token)->value,
				dollar_pos + 2,
				ft_strlen((*token)->value));
		(*token)->value = ft_strjoin_free(temp, temp2);
		free(temp2);
		dollar_pos = ft_dollar_pos((*token)->value);
	}
}

void	ft_expand_variables(t_token **token, t_list *minienv)
{
	ssize_t	dollar_pos;
	char	*var;
	char	*expanded;
	char	*temp;
	char	*temp2;

	if ((*token)->type == SINGLE_QUOTE)
		return ;
	dollar_pos = ft_dollar_pos((*token)->value);
	if ((*token)->value[dollar_pos + 1] == '?')
		return (ft_expand_exit_status(token));
	while (dollar_pos != -1)
	{
		var = ft_substr((*token)->value, dollar_pos + 1,
				ft_get_var_length((*token)->value + dollar_pos + 1));
		expanded = ft_getvar(var, minienv);
		free(var);
		temp = ft_strjoin_free(ft_substr((*token)->value, 0, dollar_pos), expanded);
		temp2 = ft_substr((*token)->value,
				dollar_pos + ft_get_var_length((*token)->value + dollar_pos + 1) + 1,
				ft_strlen((*token)->value));
		(*token)->value = ft_strjoin_free(temp, temp2);
		(*token)->variable = true; // when the value is from an expanded variable set true
		free(temp2);
		dollar_pos = ft_dollar_pos((*token)->value);
	}
}

char	*ft_getenv(char *var, t_list *minienv)
{
	t_list	*tmp;

	tmp = minienv;
	if(tmp == NULL)
		return (NULL);
	while (tmp)
	{
		if (ft_strcmp(var, tmp->key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
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

// char	*ft_get_var_value(char *var, t_list *minienv)
// {
// 	if (ft_strcmp(*var, "?") == 0)
// 		return (ft_itoa(exit_status));
// 	return (ft_getvar(var, minienv));
// }
