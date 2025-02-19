/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getcwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:00:31 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/19 11:01:00 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Minishell.h"

char	*ft_getcwd(void)
{
	char	*cwd;
	char	*prompt;
	char	*tmp;

	cwd = getcwd(NULL, 0);
	prompt = ft_strjoin("[", cwd);
	free(cwd);
	tmp = prompt;
	prompt = ft_strjoin(prompt, "]$ ");
	free(tmp);
	return (prompt);
}
