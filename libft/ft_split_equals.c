/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_equals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 18:45:16 by jarthaud          #+#    #+#             */
/*   Updated: 2023/09/18 18:05:30 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../include/minishell.h"

static int	ft_wordsize_equals(char *s)
{
	int	wsize;

	wsize = 0;
	while (s[wsize] != '=' && s[wsize])
		wsize++;
	return (wsize);
}

static void	ft_copy_bef_equals(char *res, char *s, int *i, int *j)
{
	while (s[*j] != '=' && s[*j])
		res[(*i)++] = s[(*j)++];
	res[*i] = '\0';
}

static void	ft_copy_aft_equals(char *res, char *s, int i, int j)
{
	while (s[j])
		res[i++] = s[j++];
	res[i] = '\0';
}

char	**ft_split_equals(char *s)
{
	int		i;
	int		j;
	char	**res;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	res = malloc(sizeof(char *) * 3);
	if (!res)
		return (NULL);
	res[0] = malloc(sizeof(char) * (ft_wordsize_equals(s) + 1));
	if (!res[0])
		return (free(res), NULL);
	ft_copy_bef_equals(res[0], s, &i, &j);
	i = 0;
	if (s[j] == '=')
		j++;
	res[1] = malloc(sizeof(char) * (ft_strlen(s) - j + 1));
	if (!res[1])
		return (free_tabs(res), NULL);
	ft_copy_aft_equals(res[1], s, i, j);
	res[2] = 0;
	return (res);
}
