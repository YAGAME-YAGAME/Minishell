/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:09:04 by yagame            #+#    #+#             */
/*   Updated: 2025/04/27 21:37:40 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_list **env)
{
	char	cwd[1024];

	(void)env;
	if (getcwd(NULL, 0) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (1);
}
