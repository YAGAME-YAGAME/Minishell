#include "../../includes/Minishell.h"

int	ft_dollar_pos(char *input, int i)
{
	while (input[i])
	{
		if (input[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

bool	ft_variable_expantion(char *input, t_list **lexer, int start, int *i)
{
	int	pos;
	int end;
	// t_list	*new;

	pos = ft_dollar_pos(input, start); // Find the first dollar if there is one
	if (pos == -1)
		return (false);
	else
	{
		while(input[++(*i)] && (input[*i] != input[start] || (input[*i] == input[start] && input[*i - 1] == '\\')))
		{
			end = *i;
			while(input[++(*i)] && (input[*i] != '$' || input[*i] != input[start] || (input[*i] == input[start] && input[*i - 1] == '\\')))
				end = *i;
			end = (*i)++;
			ft_lstadd_back(lexer, ft_lstnew(ft_substr(input, start, end - start + 1), SPECIAL));
			// ft_lstadd_back(lexer, ft_lstnew(ft_strdup("$"), DOLLAR));
			return (true);
		}

	}
	return (true);
}

int	ft_find_next_quote(char *input, int start)
{
	int	i;

	i = start;
	while (input[i] && (input[i] != input[start] || (input[i] == input[start] && input[i - 1] == '\\')))
		i++;
	return (i);
}

// Updated ft_add_node to handle edge cases like unclosed quotes and escaped characters
void ft_add_node(char *input, t_list **lexer, int start, int *i) {
	int end;

	end = *i;
	if (ft_strchr("|<>&;\\)\'(\"", input[*i])) { // If the current is a token
		if (input[*i] == '"' || input[*i] == '\'') { // If it's a quote
			while (input[++(*i)] && (input[*i] != input[start] || (input[*i] == input[start] && input[*i - 1] == '\\'))) {
				end = *i;
			}
			if (input[*i] != input[start]) { // Handle unclosed quotes
				printf("Error: Unclosed quote detected\n");
				ft_lstclear(lexer, free);
				return;
			}
			end = (*i)++;
			ft_lstadd_back(lexer, ft_lstnew(ft_substr(input, start, end - start + 1), SPECIAL));
			return;
		}
		while (input[*i] && ft_strchr("|<>&\\)\'(\"", input[*i])) // If it's a multiple token we take it like a single one
			end = (*i)++;
		if (input[*i] && ft_strchr(";", input[*i])) // If it's a semicolon we take it alone
			end = (*i)++;
		ft_lstadd_back(lexer, ft_lstnew(ft_substr(input, start, end - start + 1), SPECIAL));
	} else {
		while (input[*i] && !ft_strchr(" \t\n\r\a\v\f|<>&;)\'(\"", input[*i])) {
			if (input[*i] == '\\') {
				(*i)++;
				if (input[*i])
					(*i)++; // Skip escaped character
			} else
				(*i)++;
		}
		end = *i - 1;
		ft_lstadd_back(lexer, ft_lstnew(ft_substr(input, start, end - start + 1), WORD));
	}
}

t_list	*ft_create_lexer(char *input)
{
	t_list	*lexer;
	int		start;
	int		i;

	lexer = NULL;
	start = 0;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_strchr(" \t\n\r\a\v\f", input[i])) // Skip whitespaces
			i++;
		start = i;
		if (input[i] != '\0')
			ft_add_node(input, &lexer, start, &i);
		// ft_print_t_list(lexer);
		// getchar();
	}
	return (lexer);
}

void	ft_set1(t_list	**tmp)
{
	if (ft_strcmp((char *)((*tmp)->content), "|") == 0)
		(*tmp)->type = PIPE;
	else if (ft_strcmp((char *)((*tmp)->content), "$") == 0)
		(*tmp)->type = DOLLAR;
	else if (ft_strcmp((char *)((*tmp)->content), "&&") == 0)
		(*tmp)->type = AND;
	else if (ft_strcmp((char *)((*tmp)->content), "||") == 0)
		(*tmp)->type = OR;
	else if (ft_strcmp((char *)((*tmp)->content), ";") == 0)
		(*tmp)->type = SEMICOLON;
	else if (ft_strcmp((char *)((*tmp)->content), "&") == 0)
		(*tmp)->type = AMPERSAND;
	else if (ft_strcmp((char *)((*tmp)->content), "(") == 0)
		(*tmp)->type = LEFT_PARENTHESIS;
	else if (ft_strcmp((char *)((*tmp)->content), ")") == 0)
		(*tmp)->type = RIGHT_PARENTHESIS;
	else if (ft_strcmp((char *)((*tmp)->content), "<") == 0)
		(*tmp)->type = LESS_THAN;
	else if (ft_strcmp((char *)((*tmp)->content), ">") == 0)
		(*tmp)->type = GREATER_THAN;
	else if (ft_strcmp((char *)((*tmp)->content), "<<") == 0)
		(*tmp)->type = DOUBLE_LESS_THAN;
	else if (ft_strcmp((char *)((*tmp)->content), ">>") == 0)
		(*tmp)->type = DOUBLE_GREATER_THAN;
}

void	ft_set2(t_list	**tmp)
{
	if (ft_strcmp((char *)((*tmp)->content), "\'") == 0)
		(*tmp)->type = SINGLE_QUOTE;
	else if (ft_strcmp((char *)((*tmp)->content), "\"") == 0)
		(*tmp)->type = DOUBLE_QUOTE;
	else if (ft_strcmp((char *)((*tmp)->content), "\\") == 0)
		(*tmp)->type = BACKSLASH;
	else
		(*tmp)->type = NOTOKEN;
}

bool	ft_set_tokens(t_list **lexer)
{
	t_list	*tmp;

	tmp = *lexer;
	while (tmp)
	{
		if (tmp->type == SPECIAL) // If it not a normal word we need to set the exact token
		{
			ft_set1(&tmp);
			if (tmp->type == SPECIAL)
				ft_set2(&tmp);
		}
		tmp = tmp->next;
	}
	return (true);
}

void	ft_check_pipe(t_list **lstlexer)
{
	t_list *ptr;

	ptr = *lstlexer;
	while (ptr)
	{
		if (ft_strcmp(ptr->content, "|") == 0)
		{
			if (ptr->next == NULL || ptr->prev == NULL)
			{
				ft_lstclear(lstlexer, free);
				printf("PIPE SYNTAX ERROR!\n");
				exit(PIPE);
			}
		}
		ptr = ptr->next;
	}

}

void	ft_check_syntax(t_list	**lstlexer)
{
	ft_check_pipe(lstlexer);
}

// Updated ft_lexer to include better error handling and ensure proper syntax validation
t_list *ft_lexer(char *input) {
	t_list *lstlexer;

	lstlexer = ft_create_lexer(input);
	if (!lstlexer) {
		printf("Error: Lexer creation failed\n");
		return NULL;
	}
	if (!ft_set_tokens(&lstlexer)) {
		printf("Error: Token classification failed\n");
		ft_lstclear(&lstlexer, free);
		return NULL;
	}
	ft_check_syntax(&lstlexer);
	return lstlexer;
}
