/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otzarwal <otzarwal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:22:45 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/23 03:40:07 by otzarwal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_pwd(t_list *env)
{
	t_list	*tmp;

	tmp = env;
	if (ft_strcmp(tmp->key, "PWD") == 0)
		return (ft_strdup(tmp->value));
	else
		return (getcwd(NULL, 0));
	return (NULL);
}

char	*ft_getcwd(t_list *env)
{
	char	*cwd;
	char	*prompt;
	char	*tmp;

	cwd = ft_get_pwd(env);
	if (cwd && ft_strncmp(cwd, getenv("HOME"), ft_strlen(getenv("HOME"))) == 0)
		prompt = ft_strjoin(GREEN"[~",cwd + ft_strlen(getenv("HOME")));
	else
		prompt = ft_strjoin(GREEN"[", cwd);
	free(cwd);
	tmp = prompt;
	prompt = ft_strjoin(prompt, "]\n$> "RESET);
	free(tmp);
	return (prompt);
}
