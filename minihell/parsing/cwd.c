/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:22:45 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/29 09:40:31 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char 	*ft_get_pwd(t_list *env)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PWD") == 0)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*ft_getcwd(t_list *env)
{
	char	*cwd;
	char	*prompt;
	char	*tmp;

	cwd = ft_get_pwd(env);
	
	if (cwd && ft_strncmp(cwd, getenv("HOME"), ft_strlen(getenv("HOME"))) == 0)
		// prompt = ft_strjoin(CYAN"[~", cwd + ft_strlen(getenv("HOME")));
		prompt = ft_strjoin("[~", cwd + ft_strlen(getenv("HOME")));
	else
		prompt = ft_strjoin("\n[", cwd);
	free(cwd);
	tmp = prompt;
	// prompt = ft_strjoin(prompt, GREEN"]\n$>"RESET);
	prompt = ft_strjoin(prompt, "]\n$>");
	free(tmp);
	return (prompt);
}
