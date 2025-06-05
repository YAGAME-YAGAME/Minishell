/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 02:54:43 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Ensures the command array in the node has enough capacity.
 * If the needed capacity exceeds the current, attempts to resize the array.
 * On failure, prints an error and sets the global exit status.
 */
static void	ft_extend_cmd_array_if_needed(t_cmdarg **node, int needed_capacity)
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

/**
 * Joins the first split word to the last command argument if required,
 * otherwise adds the first split word as a new argument.
 */
static void	ft_join_or_add_first_word(t_cmdarg **node, char **split_words, bool first_word_joins)
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

/**
 * Adds all split words to the command array in the node.
 * Handles joining the first word if necessary and ensures enough capacity.
 */
static void	ft_add_split_words(t_cmdarg **node, char **split_words, bool first_word_joins)
{
	int		i;
	int		words_count;
	int		needed_capacity;

	words_count = 0;
	while (split_words[words_count])
		words_count++;
	needed_capacity = (*node)->cmdsize + words_count;
	if (!first_word_joins)
		needed_capacity++;
	ft_extend_cmd_array_if_needed(node, needed_capacity);
	ft_join_or_add_first_word(node, split_words, first_word_joins);
	i = 1;
	while (split_words[i])
		(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[i++]);
}

/**
 * Frees a NULL-terminated array of strings.
 */
static void	ft_free_split_words(char **split_words)
{
	int	i;

	if (!split_words)
		return ;
	i = 0;
	while (split_words[i])
		free(split_words[i++]);
	free(split_words);
}

/**
 * Handles a word that may expand into multiple words (e.g., variable expansion).
 * Splits the value, joins or adds words as needed, and frees resources.
 */
static void	ft_handle_split_word(t_cmdarg **node, t_token *token_list, char *val)
{
	char	**split_words;
	bool	should_join;

	split_words = ft_split(val, " \t");
	if (!split_words)
	{
		free(val);
		perror("malloc failure in ft_split");
		g_exit_status = 1;
		return ;
	}
	should_join = (token_list->current->prev != NULL
			&& token_list->current->prev->addspace == false);
	ft_add_split_words(node, split_words, should_join);
	ft_free_split_words(split_words);
	free(val);
}

/**
 * Ensures there is enough space in the command array for a new word.
 * Resizes the array if necessary. Returns 1 on success, 0 on failure.
 */
static int	ft_extend_cmd_for_word(t_cmdarg **node)
{
	if ((*node)->cmdsize >= (*node)->cmd_capacity)
	{
		if (!ft_resize_cmd_array(node, (*node)->cmd_capacity * 2 + 1))
		{
			perror("Failed to resize cmd array");
			g_exit_status = 1;
			return (0);
		}
	}
	return (1);
}

/**
 * Adds a word to the command array, or joins it to the previous word
 * depending on the addspace property of the previous token.
 */
static void	ft_add_or_join_word(t_cmdarg **node, t_token *token_list, char *val)
{
	char	*tmp;

	if (token_list->current->prev != NULL && token_list->current->prev->addspace == true)
		(*node)->cmd[(*node)->cmdsize++] = val;
	else
	{
		if ((*node)->cmdsize > 0)
		{
			tmp = (*node)->cmd[(*node)->cmdsize - 1];
			(*node)->cmd[(*node)->cmdsize - 1] = ft_strjoin(tmp, val);
			free(tmp);
			free(val);
		}
		else
			(*node)->cmd[(*node)->cmdsize++] = val;
	}
}

/**
 * Parses a word token and adds it to the command array in the node.
 * Handles variable expansion and splitting if necessary.
 * Returns NULL on error or after successful processing.
 */
void	*ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*val;

	val = ft_strdup(token_list->current->value);
	if (val == NULL)
		return (perror("malloc failure"), g_exit_status = 1, NULL);
	if (token_list->current->variable == true && token_list->current->type == WORD)
	{
		ft_handle_split_word(node, token_list, val);
	}
	else
	{
		if (!ft_extend_cmd_for_word(node))
		{
			free(val);
			return (NULL);
		}
		ft_add_or_join_word(node, token_list, val);
	}
	return (NULL);
}

void	ft_parse_redi(t_cmdarg **node, t_token *token_list)
{
	if (token_list->current->type == INPUT
		|| token_list->current->type == HEREDOC)
	{
		if (token_list->current->next->type == WORD)
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current, true));
		else
			ft_rediradd(&(*node)->input, ft_redinew(token_list->current,
					false));
	}
	else
		ft_rediradd(&(*node)->output, ft_redinew(token_list->current, true));
	token_list->current = token_list->current->next;
}
