/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getcwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:00:31 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/19 15:13:01 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Minishell.h"

char	*ft_getcwd(void)
{
	char	*cwd;
	char	*prompt;
	char	*tmp;

	cwd = getcwd(NULL, 0);
	if (cwd && ft_strncmp(cwd, getenv("HOME"), ft_strlen(getenv("HOME"))) == 0)
		prompt = ft_strjoin("[~", cwd + ft_strlen(getenv("HOME")));
	else
		prompt = ft_strjoin("[", cwd);
	free(cwd);
	tmp = prompt;
	prompt = ft_strjoin(prompt, "]\n$>");
	free(tmp);
	return (prompt);
}
