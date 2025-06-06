/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 21:50:27 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/06 19:13:32 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Converts token type enumeration to human-readable string.
 * Maps token type enum values to descriptive string representations
 * for debugging and visualization purposes. Helps identify what
 * type of token is being processed in the shell pipeline.
 *
 * @param type: Token type enumeration value
 * @return: String representation of the token type
 */
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

/*
 * Prints a complete token list for debugging purposes.
 * Displays all tokens in the list showing both their values
 * and types in a formatted output. Useful for debugging the
 * tokenization process and understanding the parsed structure.
 *
 * @param token_list: Head of the token linked list to display
 * Side effects: Prints formatted token information to stdout
 */
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

/*
 * Prints redirection list information for debugging.
 * Displays redirection details including type, filename, and
 * variable flag for each redirection in the list. Helps debug
 * redirection parsing and processing.
 *
 * @param redi: Head of the redirection linked list to display
 * Side effects: Prints redirection details to stdout
 */
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

/*
 * Prints complete command list structure for debugging.
 * Displays all commands in the pipeline with their arguments
 * and associated redirections. Provides comprehensive view of
 * the parsed command structure for debugging purposes.
 *
 * @param cmdarg_list: Head of the command argument linked list
 * Side effects: Prints formatted command structure to stdout
 */
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
		{
			if (tmp->cmd[i])
				printf("[%s]\n", tmp->cmd[i]);
			else
				printf("[NULL]\n");
		}
		printf("\n");
		printf("Redirections:\n");
		if (tmp->redirections)
			ft_printredi(tmp->redirections);
		tmp = tmp->next;
		printf("-------------------------------------\n\n");
	}
}
