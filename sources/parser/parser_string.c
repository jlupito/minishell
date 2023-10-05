/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:05:08 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 17:22:13 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_nb_of_args(t_lexer *lexer)
{
	int	i;

	i = 0;
	while (lexer && lexer->token_type != PIPE)
	{
		if (ft_is_string(lexer->token_type))
			i++;
		lexer = lexer->next;
	}
	return (i);
}

t_lexer	*ft_save_string(t_lexer *lexer, t_cmd *cmd, t_data *data)
{
	int	i;
	int	nb_args;

	i = 0;
	nb_args = ft_nb_of_args(lexer);
	cmd->cmd_args = malloc(sizeof(char *) * (nb_args + 1));
	if (!cmd->cmd_args)
	{
		data->mini_exit_code = MALLOC_FAIL;
		return (perror("Memory allocation failed"), NULL);
	}
	while (i < nb_args && lexer)
	{
		if (!lexer->str || lexer->token_type == HANDLED)
		{
			lexer = lexer->next;
			continue ;
		}
		cmd->cmd_args[i] = ft_strdup(lexer->str);
		lexer = lexer->next;
		i++;
	}
	cmd->cmd_args[i] = NULL;
	return (lexer);
}

t_lexer	*ft_get_strings(t_lexer *lexer, t_cmd *cmd, t_data *data)
{
	while (lexer && lexer->token_type != PIPE)
	{
		if (ft_is_string(lexer->token_type))
		{
			lexer = ft_save_string(lexer, cmd, data);
			if (!lexer)
				return (NULL);
		}
		else
			lexer = lexer->next;
		if (data->flag_pars == 1)
			return (NULL);
	}
	return (lexer);
}
