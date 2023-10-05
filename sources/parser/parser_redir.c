/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:05:02 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 17:23:00 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	ft_save_redir(t_file **files, char *name, int type, t_data *data)
{
	t_file	*new;
	t_file	*to_replace;

	new = malloc(sizeof(t_file));
	if (!new)
	{
		data->mini_exit_code = MALLOC_FAIL;
		return (perror("Memory allocation failed"), false);
	}
	new->file_name = ft_strdup(name);
	new->token_type = type;
	new->next = NULL;
	if (!*files)
	{
		*files = new;
		return (true);
	}
	to_replace = *files;
	while (to_replace->next != NULL)
		to_replace = to_replace->next;
	to_replace->next = new;
	return (true);
}

int	ft_check_redir(t_lexer *lexer, t_cmd *cmd, t_data *data)
{
	if (!lexer->next || !ft_is_string(lexer->next->token_type))
	{
		data->flag_pars = 1;
		return (1);
	}
	if (lexer->token_type == REDIR_IN || lexer->token_type == HERE_DOC)
	{
		if (!ft_save_redir(&cmd->file_in, lexer->next->str,
				lexer->token_type, data))
		{
			data->flag_pars = 1;
			return (1);
		}
	}
	else
	{
		if (!ft_save_redir(&cmd->file_out, lexer->next->str,
				lexer->token_type, data))
		{
			data->flag_pars = 1;
			return (1);
		}
	}
	return (0);
}

t_lexer	*ft_get_redir(t_lexer *lexer, t_cmd *cmd, t_data *data)
{
	t_lexer	*copy;

	copy = lexer;
	while (lexer && lexer->token_type != PIPE)
	{
		if (ft_is_redir(lexer->token_type))
		{
			if (ft_check_redir(lexer, cmd, data) == 1)
				return (NULL);
			lexer->token_type = HANDLED;
			lexer->next->token_type = HANDLED;
			lexer = lexer->next->next;
		}
		else
			lexer = lexer->next;
	}
	return (copy);
}
