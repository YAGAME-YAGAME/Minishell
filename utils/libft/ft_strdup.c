/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:47:40 by abenajib          #+#    #+#             */
/*   Updated: 2025/03/04 22:40:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *src)
{
	if (!src)
		return (NULL);
	int		slen;
	char	*dest;

	slen = 0;
	while (src[slen])
		slen++;
	dest = (char *)malloc(sizeof(char) * slen + 1);
	if (!dest)
		return (NULL);
	ft_strcpy (dest, src);
	return (dest);
}
