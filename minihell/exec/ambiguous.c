/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiguous.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:34:26 by yagame            #+#    #+#             */
/*   Updated: 2025/05/03 21:40:45 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool is_ambiguous(char *file)
{
    char *tmp;
    
    if (!file || file[0] == '\0')
        return (true);  // Empty filename is ambiguous
        
    tmp = file;
    while (*tmp)
    {
        if(*tmp == ' ')
            return (true);  // If we find a space, it's ambiguous
        tmp++;  // Move to the next character
    }
    return (false);  // No spaces found, not ambiguous
}