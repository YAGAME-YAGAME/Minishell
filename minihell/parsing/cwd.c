/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:22:45 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/19 23:31:16 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getcwd(void)
{
	char	*cwd;
	char	*prompt;
	char	*tmp;

	cwd = getcwd(NULL, 0);
	if (cwd && ft_strncmp(cwd, getenv("HOME"), ft_strlen(getenv("HOME"))) == 0)
		prompt = ft_strjoin(CYAN"[~", cwd + ft_strlen(getenv("HOME")));
	else
		prompt = ft_strjoin("\n[", cwd);
	free(cwd);
	tmp = prompt;
	prompt = ft_strjoin(prompt, GREEN"]\n$>"RESET);
	free(tmp);
	return (prompt);
}
