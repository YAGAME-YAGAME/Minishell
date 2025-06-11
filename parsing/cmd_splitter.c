/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_splitter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:27:38 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 18:28:37 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Ensures the command array in the node has enough capacity.
 * If the needed capacity exceeds the current, attempts to resize the array.
 * On failure, prints an error message and sets the global exit status
 * to indicate failure.
 *
 * @param node: Pointer to command argument node containing the command array
 * @param needed_capacity: Required capacity for the command array
 * Side effects: May resize command array, prints error on failure,
 * modifies global exit status
 */
void	ft_extend_cmd_array_if_needed(t_cmdarg **node, int needed_capacity)
{
	if (needed_capacity > (*node)->cmd_capacity)
	{
		if (!ft_resize_cmd_array(node, needed_capacity * 2))
		{
			perror("Failed to resize cmd array");
			g_exit_status = 1;
		}
	}
}

/*
 * Joins the first split word to the last command argument if required,
 * otherwise adds the first split word as a new argument.
 * Handles the special case where tokens should be concatenated without spaces.
 *
 * @param node: Pointer to command argument node to modify
 * @param split_words: Array of words from splitting operation
 * @param first_word_joins: Boolean indicating if first word should join
 * previous argument
 * Side effects: Modifies command array, allocates/frees memory
 * for string operations
 */
void	ft_join_or_add_first_word(t_cmdarg **node, char **split_words,
		bool first_word_joins)
{
	char	*tmp;

	if (first_word_joins && (*node)->cmdsize > 0)
	{
		tmp = (*node)->cmd[(*node)->cmdsize - 1];
		(*node)->cmd[(*node)->cmdsize - 1] = ft_strjoin(tmp, split_words[0]);
		free(tmp);
	}
	else
		(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[0]);
}

/*
 * Adds all split words to the command array in the node.
 * Handles joining the first word if necessary and ensures enough capacity.
 * Manages the complete process of integrating split words into
 * the command structure.
 *
 * @param node: Pointer to command argument node to modify
 * @param split_words: Array of words to add to command
 * @param first_word_joins: Boolean indicating if first word should join
 * previous argument
 * Side effects: Modifies command array size and capacity, allocates memory
 * for duplicated strings
 */
void	ft_add_split_words(t_cmdarg **node, char **split_words,
		bool first_word_joins)
{
	int		i;
	int		words_count;
	int		needed_capacity;

	if (!split_words || !split_words[0])
		return ;
	words_count = 0;
	while (split_words[words_count])
		words_count++;
	needed_capacity = (*node)->cmdsize + words_count;
	if (first_word_joins && (*node)->cmdsize > 0)
		needed_capacity--;
	ft_extend_cmd_array_if_needed(node, needed_capacity);
	ft_join_or_add_first_word(node, split_words, first_word_joins);
	i = 1;
	while (split_words[i])
	{
		(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[i]);
		i++;
	}
}

/*
 * Frees a NULL-terminated array of strings.
 * Iterates through the array, freeing each string and then the array itself.
 * Safely handles NULL arrays.
 *
 * @param split_words: Array of strings to free, may be NULL
 * Side effects: Deallocates memory for all strings and the array
 */
void	ft_free_split_words(char **split_words)
{
	int	i;

	if (!split_words)
		return ;
	i = 0;
	while (split_words[i])
		free(split_words[i++]);
	free(split_words);
}

/*
 * Handles a word that may expand into multiple words (e.g., variable expansion).
 * Splits the value by whitespace, then adds each resulting word to
 * the command array.
 * Manages token concatenation based on spacing rules and cleans up resources.
 *
 * @param node: Pointer to command argument node to modify
 * @param token_list: Token list containing current parsing context
 * @param val: String value to split and process
 * Side effects: Modifies command array, allocates/frees memory,
 * may set global exit status on error
 */
void	ft_handle_split_word(t_cmdarg **node, t_token *token_list,
		char *val)
{
	char	**split_words;
	bool	should_join;

	if (!val || !*val)
	{
		free(val);
		return ;
	}
	split_words = ft_split(val, " \t");
	if (!split_words)
	{
		free(val);
		perror("malloc failure in ft_split");
		g_exit_status = 1;
		return ;
	}
	if (!split_words[0])
		return (ft_free_split_words(split_words), free(val));
	should_join = (token_list->current->prev != NULL
			&& token_list->current->prev->addspace == false);
	ft_add_split_words(node, split_words, should_join);
	ft_free_split_words(split_words);
	free(val);
}
