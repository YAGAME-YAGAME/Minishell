/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/12 18:58:47 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_isredi(t_token *token)
{
	return (token->type == INPUT || token->type == OUTPUT
			|| token->type == HEREDOC || token->type == APPEND);
}

void	ft_parse_word(t_cmdarg **node, t_token *token_list, t_list *minienv)
{
	char	*tmp;

	ft_expand_vars(&token_list->current->value, minienv);
	tmp = (*node)->strags;//point on the old cmdargs
	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);//join the new word
	free(tmp);//free the old
	// add space between the old and the next new word
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, " ");
	free(tmp);
}

t_redi_list	*ft_redinew(t_token *token)
{
	t_redi_list	*new;

	new = malloc(sizeof(t_redi_list));
	if (!new)
		return (NULL);
	new->type = token->type;
	new->file = token->next->value;
	new->next = NULL;
	return (new);
}

void	ft_rediradd(t_redi_list **redi, t_redi_list *new)
{
	t_redi_list	*tmp;

	if (!*redi)
	{
		*redi = new;
		return ;
	}
	tmp = *redi;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_parse_redi(t_cmdarg **node, t_token *token_list)
{
	if (token_list->current->type == INPUT || token_list->current->type == HEREDOC)
		ft_rediradd(&(*node)->input, ft_redinew(token_list->current));
	else
		ft_rediradd(&(*node)->output, ft_redinew(token_list->current));
	token_list->current = token_list->current->next;//skip redi then the file after
}

t_cmdarg	*ft_init_node()
{
	t_cmdarg	*node;

	node = malloc(sizeof(t_cmdarg));
	if (!node)
		return (NULL);
	node->input = NULL;
	node->is_builtin = false;
	node->next = NULL;
	node->output = NULL;
	node->strags = NULL;
	return (node);
}

void printredi(t_redi_list *redi)
{
	t_redi_list *tmp = redi;
	while (tmp)
	{
		printf("type: {%s}, file: [%s]\n", printtype(tmp->type), tmp->file);
		tmp = tmp->next;
	}
}

void	ft_parse_squote(t_cmdarg **node, t_token *token_list)
{
	char	*tmp;

	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, "'");
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, "'");
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, " ");
	free(tmp);
}

ssize_t	ft_find_dollar(char *str)
{
	ssize_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

ssize_t	ft_find_space(char *str, ssize_t dollar_pos)
{
	ssize_t	i;

	i = dollar_pos;
	while (str[i])
	{
		if (str[i] == ' ')
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_getenv(char *var, t_list *minienv)
{
	t_list	*tmp;

	tmp = minienv;
	while (tmp)
	{
		if (ft_strncmp(var, tmp->key, ft_strlen(var)) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*ft_getvar(char *str, ssize_t dollar_pos, t_list *minienv)
{
	char	*var;
	char	*value;

	var = ft_substr(str, dollar_pos + 1, ft_find_space(str, dollar_pos + 1));
	value = ft_getenv(var, minienv);
	free(var);
	if (!value)
	{
		value = ft_strdup("");
		return (value);
	}
	return (value);
}

void	ft_expand_vars(char **value, t_list *minienv)
{
	char	*tmp;
	char	*tmp1;
	char	*expanded_value;
	ssize_t	dollar_pos;

	dollar_pos = ft_find_dollar(*value);
	tmp = *value;
	expanded_value = ft_substr(*value, 0, dollar_pos);// expanded_value = [0..$-1]
	tmp1 = expanded_value;
	expanded_value = ft_strjoin(expanded_value, ft_getvar(*value, dollar_pos, minienv));
	free(tmp1);
	*value = expanded_value;
	free(tmp);
}

void	ft_parse_dquote(t_cmdarg **node, t_token *token_list, t_list *minienv)
{
	char	*tmp;

	ft_expand_vars(&token_list->current->value, minienv);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, "\"");
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, "\"");
	free(tmp);
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, " ");
	free(tmp);
}

t_cmdarg	*get_next_node(t_token *token_list, t_list *minienv)
{
	t_cmdarg	*node;

	node = ft_init_node();
	if (!token_list->current)
		return (NULL);
	if (token_list->current->type == PIPE)
		token_list->current = token_list->current->next;
	while (token_list->current && token_list->current->type != PIPE)//inner loop on all tokens till find the pipe
	{
		if (token_list->current->type == WORD)
			ft_parse_word(&node, token_list, minienv);
		else if (ft_isredi(token_list->current))
			ft_parse_redi(&node, token_list);
		else if (token_list->current->type == SINGLE_QUOTE)
			ft_parse_squote(&node, token_list);
		else if (token_list->current->type == DOUBLE_QUOTE)
			ft_parse_dquote(&node, token_list, minienv);
		token_list->current = token_list->current->next;
	}
	return (node);
}
