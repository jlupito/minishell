/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pid_fds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:20:02 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 15:20:03 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_fd(int fd)
{
	if (fd > 2)
		close(fd);
}

void	close_next_fds(t_data *data, int i)
{
	int	j;

	j = i;
	while (++j < data->nb_cmd)
		close_all_fds(data, 1, j);
}

void	manage_redir_fds(t_data *data, t_cmd cmd)
{
	(void) data;
	if (cmd.fd_input > STDIN_FILENO)
	{
		dup2(cmd.fd_input, STDIN_FILENO);
		close(cmd.fd_input);
	}
	if (cmd.fd_output > STDOUT_FILENO)
	{
		dup2(cmd.fd_output, STDOUT_FILENO);
		close(cmd.fd_output);
	}
}

void	manage_pipe_fds(t_data *data, t_cmd cmd, int i)
{
	(void)cmd;
	if (i != data->nb_cmd - 1)
	{
		close(data->fd_pipe[0]);
		dup2(data->fd_pipe[1], STDOUT_FILENO);
		close(data->fd_pipe[1]);
	}
}
