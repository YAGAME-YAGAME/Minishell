/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:22:44 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/05 12:37:28 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	index;

	index = 0;
	if (!s || !f || *s == '\0')
		return ;
	while (s[index])
	{
		f(index, &s[index]);
		index++;
	}
}
