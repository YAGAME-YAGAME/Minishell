/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:04:33 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 02:56:56 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_printtype(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == INPUT)
		return ("INPUT");
	else if (type == OUTPUT)
		return ("OUTPUT");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	else if (type == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	return ("UNKNOWN");
}

void	ft_print_tokenlist(t_token *token_list)
{
	t_token	*tmp;

	tmp = token_list;
	printf("---------------------------------\n");
	printf("Token list:\n");
	while (tmp)
	{
		if (tmp->value)
			printf("[%s]-", tmp->value);
		tmp = tmp->next;
	}
	printf("\n");
	tmp = token_list;
	while (tmp)
	{
		printf("[%s]-", ft_printtype(tmp->type));
		tmp = tmp->next;
	}
	printf("\n");
	printf("---------------------------------\n");
}

void	ft_printredi(t_redi_list *redi)
{
	t_redi_list	*tmp;

	tmp = redi;
	while (tmp)
	{
		printf("type: {%s}, file: [%s]:%d\n", ft_printtype(tmp->type),
			tmp->file, tmp->variable);
		tmp = tmp->next;
	}
}

void	ft_printcmd_list(t_cmdarg *cmdarg_list)
{
	t_cmdarg	*tmp;
	int			i;

	tmp = cmdarg_list;
	printf("\nCMD Parser:\n");
	while (tmp)
	{
		printf("-------------------------------------\n");
		printf("Command:\n");
		i = -1;
		while (++i < tmp->cmdsize)
			printf("[%s]\n", tmp->cmd[i]);
		printf("\n");
		printf("Redirections:\n");
		if (tmp->input)
			ft_printredi(tmp->input);
		if (tmp->output)
			ft_printredi(tmp->output);
		tmp = tmp->next;
		printf("-------------------------------------\n\n");
	}
}
