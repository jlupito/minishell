/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:04:47 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 16:04:48 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_tokenize_quote(t_lexer **lexer, char *str)
{
	int		i;
	int		quote_type;
	t_token	tok;

	i = 1;
	quote_type = str[0];
	while (str[i])
	{
		if (!quote_type && ft_is_sep(str[i]))
			break ;
		if (ft_which_quote2(str[i]) && str[i] == quote_type)
			quote_type = 0;
		else if (ft_which_quote2(str[i]) && !quote_type)
			quote_type = str[i];
		i++;
	}
	if (str[0] == '\"')
		tok = DOUBLE_QUO;
	else
		tok = SINGLE_QUO;
	ft_add_back_token(lexer, ft_add_token(ft_substr(str, 0, i), tok));
	return (i + 1);
}

int	ft_tokenize_redir(t_lexer **lexer, char *str)
{
	if (str[0] == '<' && str[1] == '<')
		return (ft_add_back_token(lexer, ft_add_token(ft_substr(str, 0, 2),
					HERE_DOC)), 2);
	else if (str[0] == '<')
		return (ft_add_back_token(lexer, ft_add_token(ft_substr(str, 0, 1),
					REDIR_IN)), 1);
	else if (str[0] == '>' && str[1] == '>')
		return (ft_add_back_token(lexer, ft_add_token(ft_substr(str, 0, 2),
					REDIR_APPEND)), 2);
	else
		return (ft_add_back_token(lexer, ft_add_token(ft_substr(str, 0, 1),
					REDIR_OUT)), 1);
}

int	ft_tokenize_none(t_lexer **lexer, char *str)
{
	int		i;
	t_token	tok;

	i = 0;
	tok = NONE;
	while (str[i] && (!ft_is_sep(str[i]) || tok != NONE))
	{
		if (tok == NONE && str[i] == '\'')
			tok = SINGLE_QUO;
		else if (tok == NONE && str[i] == '"')
			tok = DOUBLE_QUO;
		else if (tok != NONE && ft_which_quote(str[i]) == tok)
			tok = NONE;
		i++;
	}
	ft_add_back_token(lexer, ft_add_token(ft_substr(str, 0, i), tok));
	return (i);
}

int	ft_tokenize(t_lexer **lexer, char *str)
{
	int	i;

	i = 0;
	while (ft_is_space(str[i]))
		i++;
	if (str[i] == '\0')
		return (i);
	else if (str[i] == '"' || str[i] == '\'')
		i += ft_tokenize_quote(lexer, &str[i]);
	else if (str[i] == '|')
	{
		ft_add_back_token(lexer, ft_add_token(ft_substr(&str[i], 0, 1), PIPE));
		i++;
	}
	else if (str[i] == '<' || str[i] == '>')
		i += ft_tokenize_redir(lexer, &str[i]);
	else
		i += ft_tokenize_none(lexer, &str[i]);
	return (i);
}
