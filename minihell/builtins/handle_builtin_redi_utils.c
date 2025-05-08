/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtin_redi_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:17:54 by codespace         #+#    #+#             */
/*   Updated: 2025/05/07 19:18:32 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_reset_std(t_cmdarg *shell)
{
	if (dup2(shell->origin_stdout, STDOUT_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	if (dup2(shell->origin_stdin, STDIN_FILENO) < 0)
	{
		write(2, "dup2 failure\n", 13);
		return ;
	}
	close(shell->origin_stdout);
	close(shell->origin_stdin);
	shell->origin_stdout = -1;
	shell->origin_stdin = -1;
	return ;
}
