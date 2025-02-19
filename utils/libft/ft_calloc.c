/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:23:31 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/19 10:54:54 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Minishell.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	totalesize;
	void	*ptr;

	totalesize = nmemb * size;
	if (nmemb != 0 && (totalesize / nmemb != size))
		return (NULL);
	ptr = malloc(totalesize);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, totalesize);
	return (ptr);
}
