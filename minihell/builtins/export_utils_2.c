/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 01:34:46 by otzarwal          #+#    #+#             */
/*   Updated: 2025/06/05 13:42:14 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Prints error message for invalid export variable identifiers.
 * Formats and displays standardized error message when export is called
 * with invalid variable names that don't conform to shell identifier rules.
 *
 * @param cmd: Invalid identifier string to include in error message
 * @return: Always returns 1 (error status)
 * Side effects: Writes error message to stderr
 */
int	print_invalid_identifier(char *cmd)
{
	write(2, "export: `", 9);
	write(2, cmd, ft_strlen(cmd));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

/*
 * Helper function for export variable declarations without assignments.
 * Sets up key-value pair when export command contains only a variable name
 * without an '=' assignment. Creates the key string and sets value to NULL.
 *
 * @param key: Pointer to store the variable key
 * @param value: Pointer to store NULL value
 * @param cmd: Command string containing the variable name
 * Side effects: Allocates memory for key string
 */
void	ft_helper(char **key, char **value, char *cmd)
{
	*key = ft_strdup(cmd);
	*value = NULL;
}

/*
 * Handles append operations for environment variables (VAR+=value).
 * Concatenates new value to existing environment variable value.
 * If the variable has no existing value, sets it to the new value.
 * Properly manages memory allocation and deallocation.
 *
 * @param dup_key: Existing environment node to append to
 * @param key: Pointer to key string (will be freed)
 * @param value: Pointer to value string to append (will be freed)
 * Side effects: Modifies environment node value, frees input strings
 */
void	ft_handle_append(t_list *dup_key, char **key, char **value)
{
	char	*tmp;

	if (!dup_key || !key || !value || !*value)
		return ;
	tmp = dup_key->value;
	if (!tmp)
		dup_key->value = ft_strdup(*value);
	else
	{
		dup_key->value = ft_strjoin(tmp, *value);
		free(tmp);
	}
	free(*key);
	free(*value);
	*key = NULL;
	*value = NULL;
}

/*
 * Removes leading and trailing whitespace from a string.
 * Creates a new string with whitespace trimmed from both ends.
 * Frees the original string and returns the trimmed copy.
 * Handles empty strings and null input gracefully.
 *
 * @param str: String to trim (will be freed)
 * @return: New trimmed string, empty string on error
 * Side effects: Frees input string, allocates new string
 */

/*
static char	*trim_spaces(char *str)
{
   char	*start;
   char	*end;
   char	*result;

   if (!str)
		return (NULL);
   if (!*str)
   {
		result = ft_strdup("");
		free(str);
		return (result);
   }
   start = str;
   while (*start && (*start == ' ' || *start == '\t'))
		start++;
   end = str + ft_strlen(str) - 1;
   while (end >= start && (*end == ' ' || *end == '\t'))
		end--;
   result = ft_substr(start, 0, end - start + 1);
   free(str);
   if (!result)
		return (ft_strdup(""));
   return (result);
}
*/

/*
 * Parses export command arguments to extract key-value pairs.
 * Handles both regular assignments (VAR=value) and
 * append operations (VAR+=value).
 * Extracts variable name and value from command string, handling proper
 * parsing of different assignment formats and whitespace trimming.
 *
 * @param cmd: Export command argument string
 * @param key: Pointer to store extracted variable key
 * @param value: Pointer to store extracted value
 * @return: 1 if append operation, 0 if regular assignment, -1 on error
 * Side effects: Allocates memory for key and value strings
 */

int	ft_handle_plus(char *cmd, char **key, char **value)
{
	int		is_append;
	char	*plus_pos;
	char	*equals_pos;

	is_append = 0;
	plus_pos = ft_strchr(cmd, '+');
	equals_pos = ft_strchr(cmd, '=');
	if (plus_pos && *(plus_pos + 1) == '=')
	{
		is_append = 1;
		*key = ft_substr(cmd, 0, plus_pos - cmd);
		*value = ft_substr(cmd, (plus_pos - cmd) + 2, ft_strlen(cmd) - (plus_pos
					- cmd) - 2);
	}
	else if (equals_pos)
		alloc_key_value(cmd, key, value, equals_pos);
	if (!*key)
	{
		if (*value)
			free(*value);
		return (-1);
	}
	return (is_append);
}
