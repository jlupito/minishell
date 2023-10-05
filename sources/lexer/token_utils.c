/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:04:38 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 16:04:39 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_is_space(int c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_is_sep(int c)
{
	return (c == '<' || c == '>' || c == '|' || ft_is_space(c));
}

int	ft_is_redir(t_token tok)
{
	return (tok == REDIR_IN || tok == REDIR_OUT
		|| tok == REDIR_APPEND || tok == HERE_DOC);
}

int	ft_is_string(t_token tok)
{
	return (tok == NONE || tok == SINGLE_QUO || tok == DOUBLE_QUO);
}

t_token	ft_which_quote(char c)
{
	if (c == '"')
		return (DOUBLE_QUO);
	else if (c == '\'')
		return (SINGLE_QUO);
	return (NONE);
}
