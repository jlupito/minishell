/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alibourb <alibourb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:05:16 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 17:15:30 by alibourb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_count_cmds(t_lexer *lexer)
{
	int				i;
	t_token			last;

	last = lexer->token_type;
	i = 1;
	while (lexer)
	{
		if (lexer->token_type == PIPE && last == PIPE)
			return (-1);
		if (lexer->token_type == PIPE && last != PIPE)
			i++;
		if (lexer->token_type != ' ' && lexer->token_type != HANDLED)
			last = lexer->token_type;
		lexer = lexer->next;
	}
	return (i);
}

t_lexer	*ft_create_cmd(t_lexer *lexer, t_cmd *cmd, t_data *data)
{
	lexer = ft_get_redir(lexer, cmd, data);
	lexer = ft_get_strings(lexer, cmd, data);
	if (cmd->cmd_args == NULL)
	{
		cmd->cmd_args = malloc(sizeof(char *));
		if (!cmd->cmd_args)
			return (perror("Memory allocation failed"), NULL);
		cmd->cmd_args[0] = NULL;
	}
	return (lexer);
}

void	ft_parse_cmd(t_data *data)
{
	int		i;
	t_lexer	*tmp_lex;

	i = 0;
	tmp_lex = data->line_lexered;
	while (data->cmds && i < data->nb_cmd)
	{
		while (tmp_lex && tmp_lex->token_type != PIPE)
		{
			tmp_lex = ft_create_cmd(tmp_lex, &data->cmds[i], data);
			if (data->flag_pars == 1)
				break ;
		}
		if (tmp_lex && tmp_lex->token_type == PIPE)
		{
			if (!tmp_lex->next)
				data->flag_pars = 1;
			tmp_lex = tmp_lex->next;
		}
		i++;
	}
}

void	ft_init_cmd(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_cmd)
	{
		data->cmds[i].file_in = NULL;
		data->cmds[i].file_out = NULL;
		data->cmds[i].heredoc = -1;
		data->cmds[i].fd_input = STDIN_FILENO;
		data->cmds[i].fd_output = STDOUT_FILENO;
		data->cmds[i].cmd_args = NULL;
		data->cmds[i].full_path = NULL;
		data->cmds[i].id_process = 0;
		i++;
	}
}

bool	ft_parser(t_data *data)
{
	data->nb_cmd = 0;
	data->flag_pars = 0;
	data->nb_cmd = ft_count_cmds(data->line_lexered);
	if (data->nb_cmd == -1)
	{
		data->flag_pars = 1;
		return (false);
	}
	data->cmds = malloc(sizeof(t_cmd) * data->nb_cmd);
	if (!data->cmds)
	{
		data->mini_exit_code = MALLOC_FAIL;
		return (perror("Memory allocation failed"), false);
	}
	ft_init_cmd(data);
	ft_parse_cmd(data);
	if (data->flag_pars)
		return (free_cmd_table(data), false);
	ft_parse_fds(data);
	free_lexer(data->line_lexered);
	return (true);
}
