/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:49:38 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 13:51:11 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_which_quote2(char c)
{
	return (c == '"' || c == '\'');
}

int	ft_count_quotes(char *str)
{
	int	quote_type;
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	quote_type = 0;
	while (str[i])
	{
		if (ft_which_quote2(str[i]))
		{
			if (!quote_type)
			{
				quote_type = str[i];
				nb++;
			}
			else if (quote_type && str[i] == quote_type)
			{
				quote_type = 0;
				nb++;
			}
		}
		i++;
	}
	return (nb);
}

void	ft_remove_quotes(char *str, char *str_replace)
{
	int	quote_type;
	int	i;
	int	j;

	i = 0;
	j = 0;
	quote_type = 0;
	while (str[i])
	{
		if (!ft_which_quote2(str[i]) || (ft_which_quote2(str[i])
				&& quote_type && quote_type != str[i]))
		{
			str_replace[j] = str[i];
			j++;
		}
		else if (!quote_type)
			quote_type = str[i];
		else if (ft_which_quote2(str[i]) && quote_type)
			quote_type = 0;
		i++;
	}
	str_replace[j] = '\0';
	free(str);
}

int	ft_check_open_quotes(char *str)
{
	int	i;
	int	quote_type;

	i = 0;
	quote_type = 0;
	while (str[i])
	{
		if (!quote_type && ft_which_quote2(str[i]))
			quote_type = str[i];
		else if (quote_type && ft_which_quote2(str[i]) && quote_type == str[i])
			quote_type = 0;
		i++;
	}
	if (quote_type)
		return (1);
	return (0);
}

char	*ft_check_remove_quotes(char *str, t_data *data)
{
	int		nb_quotes;
	char	*str_replace;

	nb_quotes = 0;
	if (ft_check_open_quotes(str))
		return (free(str), NULL);
	nb_quotes = ft_count_quotes(str);
	if (!nb_quotes)
		return (str);
	str_replace = malloc(sizeof(char) * (ft_strlen(str) - nb_quotes + 1));
	if (!str_replace)
		return (error_message(NULL, "Memory allocation failed",
				12, data), NULL);
	ft_remove_quotes(str, str_replace);
	return (str_replace);
}
