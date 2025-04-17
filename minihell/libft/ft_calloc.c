/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:23:31 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/05 12:37:28 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
