/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 01:08:06 by yagame            #+#    #+#             */
/*   Updated: 2025/06/05 14:06:48 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Validates that a string contains only 'n' characters after "-".
 * Used to verify that echo -n flags contain only 'n' characters,
 * ensuring proper parsing of the no-newline option.
 *
 * @param str: String to validate (should start with "-n")
 * @return: 1 if string contains only 'n' after "-", 0 otherwise
 */
int	only_n(char *str)
{
	int	i;

	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/*
 * Joins command arguments into a single string with spaces.
 * Concatenates all arguments starting from the specified index,
 * inserting spaces between arguments to form a complete output string
 * for the echo command.
 *
 * @param cmd: Array of command arguments
 * @param i: Starting index for joining arguments
 * @return: Joined string containing all arguments separated by spaces
 * Side effects: Allocates memory for result string
 */
char	*ft_join_arg(char **cmd, int i)
{
	char	*tmp;
	char	*hold;

	hold = ft_strdup("");
	while (cmd[i])
	{
		tmp = ft_strjoin(hold, cmd[i]);
		free(hold);
		hold = tmp;
		if (cmd[i + 1])
		{
			tmp = ft_strjoin(hold, " ");
			free(hold);
			hold = tmp;
		}
		i++;
	}
	return (hold);
}

/*
 * Outputs the echo string with or without a trailing newline.
 * Prints the given string to stdout, adding a newline unless
 * the -n flag is specified, which suppresses the newline.
 *
 * @param tmp: String to output
 * @param n_flag: If non-zero, suppresses trailing newline
 * Side effects: Writes to stdout
 */
void	ft_print_echo(char *tmp, int n_flag)
{
	if (!n_flag)
		ft_putendl_fd(tmp, 1);
	else
		ft_putstr_fd(tmp, 1);
}

/*
 * Implements the echo builtin command.
 * Outputs the specified arguments to stdout, supporting the -n option
 * to suppress the trailing newline. Handles multiple -n flags and
 * validates flag format (only 'n' characters allowed after '-n').
 *
 * @param cmd: Array of command arguments (cmd[0] is "echo")
 * @param shell: Command argument structure (unused)
 * @return: Always returns 0 (success)
 * Side effects: Writes to stdout, allocates/frees memory for string operations
 */
int	ft_echo(char **cmd, t_cmdarg *shell)
{
	int		i;
	int		n_flag;
	char	*tmp;

	(void)shell;
	i = 1;
	n_flag = 0;
	tmp = NULL;
	while (cmd[i] && ft_strncmp(cmd[i], "-n", 2) == 0)
	{
		if (!only_n(cmd[i]))
			break ;
		n_flag = 1;
		i++;
	}
	tmp = ft_join_arg(cmd, i);
	ft_print_echo(tmp, n_flag);
	free(tmp);
	return (0);
}
