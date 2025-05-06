/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 20:04:35 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/05 12:37:28 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int					sign;
	unsigned long long	result;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
		if (sign == 1 && result > (9223372036854775807))
			return (-1);
		else if (sign == -1 && result > (9223372036854775807))
			return (0);
	}
	return ((int)(result * sign));
}
// #include <stdio.h>
// #include <xlocale.h>
// int main()
// {
// 	char s[] = "2233720368547758085555555555";
// 	int a = ft_atoi(s);
// 	int at = atoi(s);
// 	if (a == at)
// 		printf("test pass");
// 	else
// 		printf("atoi:%d\nft_atoi:%d\n", at, a);
// }
