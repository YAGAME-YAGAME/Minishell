/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 09:38:40 by abenajib          #+#    #+#             */
/*   Updated: 2025/02/19 12:28:34 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	temp;

	if (!haystack && len == 0 && needle)
		return (NULL);
	i = 0;
	temp = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	while (haystack[i] && i < len)
	{
		j = 0;
		temp = i;
		while ((haystack[temp] == needle[j])
			&& (temp < len && haystack[temp] && needle[j]))
		{
			j++;
			temp++;
		}
		if (needle[j] == '\0')
			return ((char *)(haystack + i));
		i++;
	}
	return (NULL);
}
