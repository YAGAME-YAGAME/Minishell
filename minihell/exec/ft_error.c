/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:52:05 by otzarwal          #+#    #+#             */
/*   Updated: 2025/05/05 15:15:52 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_cmd_error(char *cmd_name, char *error, int status)
{
    write(2, "minishell : ", 11);
    if (cmd_name)
    {
        write(2, cmd_name, ft_strlen(cmd_name));
        write(2, ": ", 2);
    }
	perror (error);
    // write(2, error, ft_strlen(error));
    exit(status);
}

void	ft_error(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}


int ft_open_file(char *file, int flag)
{
	int fd;
	if(flag == 0)
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if(flag == 1)
		fd = open(file, O_RDONLY);
	else
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		ft_cmd_error(file, NULL, 1);
	return (fd);
}

// void 	ft_free_list(t_list **list)
// {
// 	if (!list || !*list)
// 		return ;
// 	if((*list)->key)
// 		free((*list)->key);
// 	if((*list)->value)
// 		free((*list)->value);
// 	free(*list);
// }

// char	*find_path(t_list *path)
// {
// 	t_list *tmp;

// 	tmp = path;
// 	while (tmp)
// 	{
// 		if (!ft_strncmp(tmp->key, "PATH", 4))
// 			return (tmp->value);
// 		tmp = tmp->next;
// 	}
// 	return (NULL);
// }

// void  ft_get_path(char **full_path, char **path_cmd, char *p)
// {
//     char *tmp;

//     tmp = NULL;
//     *full_path = NULL;
//     while (*path_cmd)
//     {
//         *full_path = ft_strjoin(*path_cmd, "/");
//         if (!*full_path)
//             return;
        
//         tmp = *full_path;
//         *full_path = ft_strjoin(tmp, p);
//         free(tmp);
//         tmp = NULL;
        
//         if (!*full_path)
//             return;
        
//         if (access(*full_path, X_OK) == 0)
//             return;  // Found executable
        
//         free(*full_path);
//         *full_path = NULL;
//         path_cmd++;
//     }
// }

// char	*check_exec(char *p, t_list *env)
// {
// 	char	*full_path;
// 	char	*path;
// 	char	**path_cmd;

// 	if (!p)
// 		return (NULL);
// 	full_path = NULL;
// 	if (*p == '/' || *p == '.')
// 	{
// 		if (access(p, X_OK) == 0)
// 			return (ft_strdup(p));
// 		else
// 			return (NULL);
// 	}
// 	path = find_path(env);
// 	if (!path)
// 		return (NULL);
// 	path_cmd = parsing_split(path, ':');
// 	ft_get_path(&full_path, path_cmd, p);
// 	free_dp(path_cmd);
// 	return (full_path);
// }
