/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:49:29 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 11:07:18 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_check_expansion(char **str, t_lexer *lex, t_data *data)
{
	int	i;
	int	j;
	int	quote_type;

	i = 0;
	quote_type = 0;
	while ((*str)[i])
	{
		if (ft_which_quote2((*str)[i]) && !quote_type)
			quote_type = (*str)[i];
		else if (ft_which_quote2((*str)[i]) && quote_type == (*str)[i])
			quote_type = 0;
		else if ((*str)[i] == '$' && quote_type != '\'')
		{
			j = ft_expand_var(str, i + 1, quote_type, data);
			lex->expanded = 1;
			i += j - 1;
		}
		i++;
	}
}

int	ft_to_be_split(char *str)
{
	int	countw;
	int	i;

	i = 0;
	countw = 1;
	while (str[i])
	{
		if (ft_is_space(str[i]) && str[i])
		{
			countw++;
			while (ft_is_space(str[i]) && str[i])
				i++;
		}
		else
			i++;
	}
	return (countw);
}

void	ft_split_add_token(char **str, t_lexer *lex, int i)
{
	t_lexer	*new;
	t_lexer	*second;

	if (i == 0)
	{
		free(lex->str);
		lex->str = ft_strdup(str[i]);
		free(str[i]);
	}
	else
	{
		second = NULL;
		if (lex->next)
			second = lex->next;
		new = ft_add_token(str[i], NONE);
		lex->next = new;
		new->prev = lex;
		if (second)
		{
			new->next = second;
			second->prev = new;
		}
	}
}

void	ft_split_args(char *str, t_lexer *lex)
{
	char	**tmp_str;
	int		i;
	int		nb_args;

	i = -1;
	tmp_str = ft_split(str, ' ');
	if (!tmp_str)
		free_tabs(tmp_str);
	nb_args = ft_to_be_split(str);
	while (++i < nb_args)
		ft_split_add_token(tmp_str, lex, i);
	free(tmp_str);
}

bool	ft_expand(t_data *data)
{
	t_lexer	*tmp_lex;

	tmp_lex = data->line_lexered;
	while (tmp_lex)
	{
		if (ft_is_string(tmp_lex->token_type))
		{
			if (!tmp_lex->prev || (tmp_lex->prev
					&& tmp_lex->prev->token_type != HERE_DOC))
				ft_check_expansion(&tmp_lex->str, tmp_lex, data);
			if (tmp_lex->expanded && ft_to_be_split(tmp_lex->str) > 1)
				ft_split_args(tmp_lex->str, tmp_lex);
			tmp_lex->str = ft_check_remove_quotes(tmp_lex->str, data);
			if (!tmp_lex->str)
				return (error_message("Unmatched quote",
						"cannot access '' or \"\"", 1, data), false);
		}
		tmp_lex = tmp_lex->next;
	}
	return (true);
}
