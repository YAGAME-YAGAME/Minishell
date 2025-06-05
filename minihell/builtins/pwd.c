/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:09:04 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 03:22:33 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Implements the pwd builtin command.
 * Prints the current working directory path to stdout using the getcwd
 * system call. Handles errors by printing an error message via perror.
 *
 * @param env: Pointer to environment variables list (unused)
 * @return: Always returns 1
 * Side effects: Writes current directory to stdout, may write error to stderr
 */
int	ft_pwd(t_list **env)
{
	char	cwd[1024];

	(void)env;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (1);
}
