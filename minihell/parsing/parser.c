/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/06/05 02:01:24 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_add_split_words(t_cmdarg **node, char **split_words, bool first_word_joins)
{
	int		i;
	char	*tmp;
	int		words_count;
	int		needed_capacity;

	// Count words in split_words
	words_count = 0;
	while (split_words[words_count])
		words_count++;

	// Calculate needed capacity
	needed_capacity = (*node)->cmdsize + words_count;
	if (!first_word_joins)
		needed_capacity++; // Account for one less joining

	// Resize if necessary
	if (needed_capacity > (*node)->cmd_capacity)
	{
		if (!ft_resize_cmd_array(node, needed_capacity * 2))
		{
			perror("Failed to resize cmd array");
			g_exit_status = 1;
			return ;
		}
	}

	i = 0;
	while (split_words[i])
	{
		if (i == 0 && first_word_joins)
		{
			// First word joins with previous argument if needed
			if ((*node)->cmdsize > 0)
			{
				tmp = (*node)->cmd[(*node)->cmdsize - 1];
				(*node)->cmd[(*node)->cmdsize - 1] = ft_strjoin(tmp, split_words[i]);
				free(tmp);
			}
			else
				(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[i]);
		}
		else
		{
			// All other words become separate arguments
			(*node)->cmd[(*node)->cmdsize++] = ft_strdup(split_words[i]);
		}
		i++;
	}
}

static void	ft_free_split_words(char **split_words)
{
	int	i;

	if (!split_words)
		return ;
	i = 0;
	while (split_words[i])
	{
		free(split_words[i]);
		i++;
	}
	free(split_words);
}

void	*ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*val;
	char	*tmp;
	char	**split_words;
	bool	should_join;

	tmp = NULL;
	val = ft_strdup(token_list->current->value);
	if (val == NULL)
		return (perror("malloc failure"), g_exit_status = 1, NULL);

	// Check if this token is result of variable expansion and is a WORD token
	if (token_list->current->variable == true && token_list->current->type == WORD)
	{
		// Split the expanded value by spaces and tabs
		split_words = ft_split(val, " \t");
		if (!split_words)
		{
			free(val);
			return (perror("malloc failure in ft_split"), g_exit_status = 1, NULL);
		}

		// Determine if first word should join with previous argument
		should_join = (token_list->current->prev != NULL
			&& token_list->current->prev->addspace == false);

		// Add all split words to command arguments
		ft_add_split_words(node, split_words, should_join);

		// Clean up
		ft_free_split_words(split_words);
		free(val);
	}
	else
	{
		// Original logic for non-variable tokens or non-WORD tokens
		// Ensure we have capacity for at least one more argument
		if ((*node)->cmdsize >= (*node)->cmd_capacity)
		{
			if (!ft_resize_cmd_array(node, (*node)->cmd_capacity * 2 + 1))
			{
				free(val);
				perror("Failed to resize cmd array");
				g_exit_status = 1;
				return (NULL);
			}
		}

		if (token_list->current->prev != NULL
			&& token_list->current->prev->addspace == true)
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
