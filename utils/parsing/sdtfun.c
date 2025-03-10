#include "../../includes/Minishell.h"

char	**ft_strdup_2d(char **str)
{
	int		i;
	char	**new_str;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
		i++;
	new_str = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		new_str[i] = ft_strdup(str[i]);
		i++;
	}
	new_str[i] = NULL;
	return (new_str);
}

void	ft_free_2d(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_print_t_list(t_list *list)
{
	printf("\n");
	while (list)
	{
		printf("[%s] ", (char *)list->content);
		list = list->next;
	}
	printf("\n");
}
