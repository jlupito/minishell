/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:56:21 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 10:08:12 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_lexer	*ft_add_token(char *str1, t_token token)
{
	t_lexer	*new;

	new = malloc(sizeof(t_lexer));
	if (!new)
		return (perror("Memory allocation failed"), NULL);
	new->str = ft_strdup2(str1);
	new->token_type = token;
	new->expanded = 0;
	new->next = NULL;
	new->prev = NULL;
	free(str1);
	return (new);
}

void	ft_add_back_token(t_lexer **lst, t_lexer *new)
{
	t_lexer	*add_last;

	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	add_last = *lst;
	while (add_last->next)
		add_last = add_last->next;
	add_last->next = new;
	new->prev = add_last;
}

t_lexer	*ft_lexer(char *str)
{
	unsigned int	i;
	t_lexer			*tmp;

	i = 0;
	tmp = NULL;
	while (i < ft_strlen(str))
		i += ft_tokenize(&tmp, &str[i]);
	return (tmp);
}

int	ft_lexer_parse_msg(t_lexer *lex_copy, t_data *data)
{
	if (lex_copy->token_type < 4 && !lex_copy->next)
		return (error_message("'newline'",
				"synthax error near unexpected token", errno, data), 1);
	else if (lex_copy->token_type < 4 && lex_copy->next->token_type < 4)
		return (error_message(lex_copy->str,
				"synthax error near unexpected token", errno, data), 1);
	else if (lex_copy->token_type == PIPE && !lex_copy->next)
		return (error_message(lex_copy->str,
				"synthax error near unexpected token", errno, data), 1);
	return (0);
}

int	ft_lexer_parse(t_data *data, char *str)
{
	t_lexer	*lex_copy;

	data->line_lexered = ft_lexer(str);
	free(str);
	if (!data->line_lexered)
	{
		data->mini_exit_code = MALLOC_FAIL;
		return (1);
	}
	lex_copy = data->line_lexered;
	if (lex_copy->token_type == NONE && !lex_copy->next)
		return (ft_check_particular(data));
	if (lex_copy->token_type == PIPE)
		return (error_message(lex_copy->str,
				"synthax error near unexpected token", 2, data), 1);
	while (lex_copy)
	{
		if (ft_lexer_parse_msg(lex_copy, data))
		{
			data->mini_exit_code = 2;
			return (1);
		}
		lex_copy = lex_copy->next;
	}
	return (0);
}
