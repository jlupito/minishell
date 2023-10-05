/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alibourb <alibourb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 16:05:08 by jarthaud          #+#    #+#             */
/*   Updated: 2023/09/20 16:51:35 by alibourb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	res;
	int	neg;

	i = 0;
	neg = 1;
	while (nptr[i] && (nptr[i] == 9 || nptr[i] == 10 || nptr[i] == 11
			|| nptr[i] == 12 || nptr[i] == 13 || nptr[i] == 32))
		i++;
	if (nptr[i] == 43 || nptr[i] == 45)
	{
		if (nptr[i] == 45)
			neg = -neg;
		i++;
	}
	res = 0;
	while (nptr[i] && (nptr[i] > 47 && nptr[i] < 58))
	{
		res = res * 10 + (nptr[i] - 48);
		i++;
	}
	res *= neg;
	return (res);
}
