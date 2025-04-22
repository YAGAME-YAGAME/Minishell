/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:55:24 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/21 20:18:41 by yagame           ###   ########.fr       */
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

int is_builtin(char *cmd)
{
    if (ft_strcmp(cmd, "echo") == 0)
        return (0);
    else if (ft_strcmp(cmd, "cd") == 0)
        return (0);
    else if (ft_strcmp(cmd, "pwd") == 0)
        return (0);
    else if (ft_strcmp(cmd, "clear") == 0)
        return (0);
    else if (ft_strcmp(cmd, "export") == 0)
        return (0);
    else if (ft_strcmp(cmd, "unset") == 0)
        return (0);
    else if (ft_strcmp(cmd, "env") == 0)
        return (0);
    else if (ft_strcmp(cmd, "exit") == 0)
        return (0);
    return (-1);
}
int    run_built_in(t_cmdarg *shell, t_list *env, char *input)
{
    char **cmd;
    
    cmd = handel_quote(parsing_split(shell->strags, ' '));
    if(cmd == NULL)
        return (1);
    if(is_builtin(cmd[0]) == 0)
    {
        // must be free cmd;
        if (ft_strcmp(cmd[0], "echo") == 0)
            return (ft_echo(cmd, shell));
        else if (ft_strcmp(cmd[0], "cd") == 0)
            return (ft_cd(cmd, env));
        else if (ft_strcmp(cmd[0], "pwd") == 0)
            return (ft_pwd(cmd, env));
        else if (ft_strcmp(cmd[0], "clear") == 0)
            return (ft_clear());
        else if (ft_strcmp(cmd[0], "export") == 0)
            return (ft_export(cmd, &env));
        else if (ft_strcmp(cmd[0], "unset") == 0)
            return (ft_unset(cmd, env));
        else if (ft_strcmp(cmd[0], "env") == 0)
            return (ft_env(env));
        else if (ft_strcmp(cmd[0], "exit") == 0)
                ft_exit(env, input);
    }
    return (0);
}
