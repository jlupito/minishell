/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alibourb <alibourb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:04:55 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 11:33:45 by alibourb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_parse_fds_in(t_file *tmp_in, t_cmd *cmd, t_data *data)
{
	while (tmp_in)
	{
		if (cmd->fd_input > 0)
			close (cmd->fd_input);
		if (tmp_in->token_type == HERE_DOC)
			cmd->fd_input = cmd->heredoc;
		else if (tmp_in->token_type == REDIR_IN)
			cmd->fd_input = open(tmp_in->file_name, O_RDONLY);
		if (cmd->fd_input == FAILURE)
		{
			if (access(tmp_in->file_name, F_OK) < 0)
			{
				error_message("No such file or directory",
					tmp_in->file_name, 1, data);
				return ;
			}
			error_message("Permission denied", tmp_in->file_name, 1, data);
			return ;
		}
		tmp_in = tmp_in->next;
	}
}

void	ft_parse_fds_out(t_file *tmp_out, t_cmd *cmd, t_data *data)
{
	while (tmp_out)
	{
		if (cmd->fd_output > 2)
			close (cmd->fd_output);
		if (tmp_out->token_type == REDIR_OUT)
			cmd->fd_output = open(tmp_out->file_name, O_CREAT | O_WRONLY
					| O_TRUNC, 0644);
		else if (tmp_out->token_type == REDIR_APPEND)
			cmd->fd_output = open(tmp_out->file_name, O_CREAT | O_WRONLY
					| O_APPEND, 0644);
		if (cmd->fd_output == FAILURE)
			return (error_message("Permission denied",
					tmp_out->file_name, 1, data));
		tmp_out = tmp_out->next;
	}
}

bool	ft_parse_fds(t_data *data)
{
	int		i;
	t_file	*tmp_in;
	t_file	*tmp_out;

	i = 0;
	check_heredoc(data);
	while (i < data->nb_cmd)
	{
		tmp_in = data->cmds[i].file_in;
		tmp_out = data->cmds[i].file_out;
		if (tmp_in)
			ft_parse_fds_in(tmp_in, &data->cmds[i], data);
		if (tmp_out)
			ft_parse_fds_out(tmp_out, &data->cmds[i], data);
		if (errno)
			return (false);
		i++;
	}
	return (true);
}
