/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:30:41 by yagame            #+#    #+#             */
/*   Updated: 2025/04/18 10:33:27 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int     handel_output(t_redi_list *output)
{
    int fd;
    
    fd = -1;
    while(output)
    {
        fd = open(output->file, O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("Error opening file");
            return (-1);
        }
        if(output->is_last)
            return (fd);
        close(fd);
        output = output->next;
    }
    return (-1);
}