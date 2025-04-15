/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 18:51:11 by otzarwal          #+#    #+#             */
/*   Updated: 2025/04/15 19:12:57 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void    handle_execution(t_cmdarg *current_cmd, t_list *env)
{
    char **cmd;
    char *cmd_path;
    
    cmd = parsing_split(current_cmd->strags, ' ');
    char **tmp = cmd;
    while(*tmp)
        printf("[ %s ]\n", *tmp++);
    cmd_path = check_exec(cmd[0], env);
    if(!cmd_path)
    {
          perror("command not found\n");
          exit(EXIT_FAILURE);
    }
    char **envp = NULL;
    envp = get_env(env);
    if(execve(cmd_path, cmd, envp) == -1)
    {
        perror("execve failure\n");
        free(cmd_path);
        free(envp);
        exit(EXIT_FAILURE);
    }
}

void    handle_heredoc(t_redi_list *input)
{
    int heredoc_fd[2];
    
    if(pipe(heredoc_fd) == -1)
    {
        perror("pipe failure\n");
        exit(EXIT_FAILURE);
    }
    write(heredoc_fd[1], input->content, ft_strlen(input->content));
    close(heredoc_fd[1]);
    if(input->is_last)
    {
        dup2(heredoc_fd[0], STDIN_FILENO);
        close(heredoc_fd[0]);
    }
    else
        close(heredoc_fd[0]);
    free(input->content);
}


void handel_append(t_redi_list *output)
{
    int out_fd;
    
    out_fd = open(output->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(out_fd == -1)
        {
            perror("output file not found\n");
            exit(EXIT_FAILURE);
        }
        if(output->is_last)
        {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        else
            close(out_fd);
}


void    handle_output(t_redi_list *output)
 {
    int out_fd;
    
    while(output)
   {
        if(output->type == OUTPUT)
        {
            out_fd = open(output->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if(out_fd == -1)
            {
                perror("output file not found\n");
                exit(EXIT_FAILURE);
            }
            if(output->is_last)
            {
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }
            else
                close(out_fd);
        }
        if(output->type == APPEND)
            handel_append(output);
        output = output->next;
    }
 }


void handle_input(t_redi_list *input)
{
    int in_fd;
    
    while(input)
    {
        if(input->type == INPUT)
        {
            in_fd = open(input->file, O_RDONLY);
            if(in_fd == -1)
            {
                perror("input file not found\n");
                exit(EXIT_FAILURE);
            }
            if(input->is_last)
            {
                printf("is last redirection : %s\n", input->file);
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            else
                close(in_fd);
        }
        if(input->type == HEREDOC && input->content)
            handle_heredoc(input);
        input = input->next;
    }
}
// =====================/ end handle input redirection /========================//

void    ft_child(t_cmdarg *current_cmd, t_list *env, int tmp_in, int *p_fd)
{
    if(tmp_in != 0)
    {
            dup2(tmp_in, STDIN_FILENO);
            close(tmp_in);
    }
    if(current_cmd->next)
    {
            dup2(p_fd[1], STDOUT_FILENO);
            close(p_fd[1]);
    }
    handle_input(current_cmd->input);
    handle_output(current_cmd->output);
    handle_execution(current_cmd, env);

    
}

