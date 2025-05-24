/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:14:53 by abenajib          #+#    #+#             */
/*   Updated: 2025/05/24 12:29:59 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_redierrors(t_token *current)
{
	return (ft_isredi(current) && (current->next == NULL
			|| ft_isredi(current->next)));
}

bool	ft_pipeerrors(t_token *current)
{
	return (current->type == PIPE && ((current->next == NULL
				|| current->next->type == PIPE || current->prev == NULL
				|| ft_isredi(current->prev))));
}

void	ft_cleaner(t_token *token_list, t_cmdarg *cmdarg_list)
{
	ft_free_tokenlist(token_list);
	ft_free_cmdlist(cmdarg_list);
}

// void	ft_printcmd_list(t_cmdarg *cmdarg_list)
// {
// 	t_cmdarg	*tmp;

// 	tmp = cmdarg_list;
// 	printf("\nCMD Parser:\n");
// 	while (tmp)
// 	{
// 		printf("-------------------------------------\n");
// 		printf("Command:\n");
// 		for (int i = 0; i < tmp->cmdsize; i++)
// 			printf("[%s]\n", tmp->cmd[i]);
// 		printf("\n");
// 		printf("Redirections:\n");
// 		if (tmp->input)
// 			ft_printredi(tmp->input);
// 		if (tmp->output)
// 			ft_printredi(tmp->output);
// 		tmp = tmp->next;
// 		printf("-------------------------------------\n\n");
// 	}
// }
