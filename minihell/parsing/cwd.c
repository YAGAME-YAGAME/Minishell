/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:22:45 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/06 21:10:01 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * @brief Retrieves the current working directory from the environment list or system.
 * @param env The environment variable list.
 * @return A newly allocated string with the current working directory.
 */
char	*ft_get_pwd(t_list *env)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PWD") == 0)
			return (ft_strdup(tmp->value));
		else
			return (getcwd(NULL, 0));
		tmp = tmp->next;
	}
	return (NULL);
}

/**
 * @brief Builds the shell prompt string based on the current working directory.
 * @param env The environment variable list.
 * @return A newly allocated prompt string.
 */
char	*ft_getcwd(t_list *env)
{
	char	*cwd;
	char	*prompt;
	char	*tmp;

	cwd = ft_get_pwd(env);
	if (cwd && ft_strncmp(cwd, getenv("HOME"), ft_strlen(getenv("HOME"))) == 0)
		prompt = ft_strjoin("[~", cwd + ft_strlen(getenv("HOME")));
	else
		prompt = ft_strjoin("[", cwd);
	free(cwd);
	tmp = prompt;
	prompt = ft_strjoin(prompt, "]\n$> ");
	free(tmp);
	return (prompt);
}
