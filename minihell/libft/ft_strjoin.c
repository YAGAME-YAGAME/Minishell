/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 18:28:38 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/05 15:48:59 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_check_path(char **paths, char *cmd)
{
	char	*full_path;
	char	*path;

	path = ft_strjoin(*paths, "/");
	full_path = ft_strjoin(path, cmd);
	free(path);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*ft_path_checker(char *cmd)
{
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

char	*ft_find_path(char *cmd, char *envp[])
{
	char	**paths;
	int		i;
	char	*checker;
	char	**holder;

	i = -1;
	if (ft_strchr(cmd, '/'))
		return (ft_path_checker(cmd));
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ":");
			holder = paths;
			while (*paths)
			{
				checker = ft_check_path(paths++, cmd);
				if (checker)
					return (checker);
			}
			freeall(holder, countwords(envp[i] + 5, ":"));
			break ;
		}
	}
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		return (ft_strdup(s2));
	if (s1 && !s2)
		return (ft_strdup(s1));
	i = 0;
	j = 0;
	str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}
