/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varexp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:34:36 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Expands special variables like exit status ($?) and process ID ($$).
 * Handles the expansion of '$?' to the last exit status and '$$' to the
 * current process ID. Replaces the variable in the token value and marks
 * the token as containing an expanded variable.
 *
 * @param token: Pointer to token containing the variable to expand
 * Side effects: Modifies token value, sets variable flag, allocates/frees memory
 */
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

/*
 * Performs normal environment variable expansion for a token.
 * Handles the expansion of regular environment variables by looking up
 * their values and replacing them in the token. Manages string reconstruction
 * around the variable position.
 *
 * @param token: Pointer to token containing the variable to expand
 * @param minienv: Environment variables list for lookup
 * @param dpos: Position of the dollar sign in the token value
 * Side effects: Modifies token value, sets variable flag, allocates/frees memory
 */
void	ft_expander_norm(t_token **token, t_list *minienv, ssize_t dpos)
{
	char	*exp;
	char	*temp;
	char	*temp2;

	exp = ft_expand(token, minienv, dpos);
	temp = ft_strjoin_free(ft_substr((*token)->value, 0, dpos), exp);
	temp2 = ft_temp2(token, dpos);
	free((*token)->value);
	(*token)->value = ft_strjoin_free(temp, temp2);
	free(temp2);
	(*token)->variable = true;
	free(exp);
}

/*
 * Expands all environment variables found in a token.
 * Main variable expansion function that processes a token and replaces
 * all expandable variables with their values. Skips expansion for single-quoted
 * tokens and handles both special variables and regular environment variables.
 *
 * @param token: Pointer to token to process for variable expansion
 * @param minienv: Environment variables list for variable lookup
 * Side effects: Modifies token value and variable flag, allocates/frees memory
 */
void	ft_expand_variables(t_token **token, t_list *minienv)
{
	ssize_t	dpos;

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
		ft_expander_norm(token, minienv, dpos);
		dpos = ft_dollar_pos((*token)->value);
	}
}

/*
 * Retrieves the value of an environment variable by name.
 * Searches through the environment linked list for a variable with
 * the specified key and returns a duplicate of its value if found.
 *
 * @param var: Name of the environment variable to look up
 * @param minienv: Environment variables linked list
 * @return: Duplicated string value of the variable, NULL if not found
 * Side effects: Allocates memory for returned string
 */
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

/*
 * Retrieves an environment variable value or returns empty string.
 * Wrapper function around ft_getenv that ensures a non-null return value.
 * If the variable is not found, returns an empty string instead of NULL.
 *
 * @param var: Name of the environment variable to look up
 * @param minienv: Environment variables linked list
 * @return: Variable value or empty string, never NULL
 * Side effects: Allocates memory for returned string
 */
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
