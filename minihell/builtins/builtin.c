/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:55:24 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/18 23:00:27 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void    free_dp(char **cmd)
{
    int i;

    i = 0;
    while (cmd[i])
    {
        free(cmd[i]);
        i++;
    }
    free(cmd);
}

char **handel_quote(char **cmd)
{
    char **trim;
    int i;

    i = 0;
    while(cmd[i])
        i++;
    trim = malloc(sizeof(char *) * (i + 1));
    if (!trim)
        return (NULL);
    i = 0;
    while (cmd[i])
    {
        trim[i] = ft_strtrim(cmd[i], "\"");
        if (!trim[i])
            return (NULL);
        i++;
    }
    trim[i] = NULL;
    free_dp(cmd);
    return (trim);
}

int is_builtin(char *cmd)
{
    if (ft_strncmp(cmd, "echo", 4) == 0)
        return (0);
    else if (ft_strncmp(cmd, "cd", 2) == 0)
        return (0);
    else if (ft_strncmp(cmd, "pwd", 3) == 0)
        return (0);
    else if (ft_strncmp(cmd, "clear", 5) == 0)
        return (0);
    else if (ft_strncmp(cmd, "export", 6) == 0)
        return (0);
    else if (ft_strncmp(cmd, "unset", 5) == 0)
        return (0);
    else if (ft_strncmp(cmd, "env", 3) == 0)
        return (0);
    else if (ft_strncmp(cmd, "exit", 4) == 0)
        return (0);
    return (-1);
}
int    run_built_in(t_cmdarg *shell, t_list *env, char *input)
{
    char **envp;
    char **cmd;


    envp = get_env(env);
    cmd = handel_quote(parsing_split(shell->strags, ' '));
    
    if (cmd == NULL)
        return (1);
    if(is_builtin(cmd[0]) == 0)
    {
        if (ft_strncmp(cmd[0], "echo", 4) == 0)
            return (ft_echo(cmd, shell));
        else if (ft_strncmp(cmd[0], "cd", 2) == 0)
            return (ft_cd(cmd, env));
        else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
            return (ft_pwd(cmd, envp));
        else if (ft_strncmp(cmd[0], "clear", 5) == 0)
            return (ft_clear(cmd, envp));
        else if (ft_strncmp(cmd[0], "export", 6) == 0)
            return (ft_export(cmd, &env));
        else if (ft_strncmp(cmd[0], "unset", 5) == 0)
            return (ft_unset(cmd, env));
        else if (ft_strncmp(cmd[0], "env", 3) == 0)
            return (ft_env(env));
        else if (ft_strncmp(cmd[0], "exit", 4) == 0)
                ft_exit(env, input);
    }
    free_dp(cmd);
    return (0);
}
