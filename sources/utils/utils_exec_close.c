/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec_close.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:08:06 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 16:08:07 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_all_fds(t_data *data, int flag, int i)
{
	int		j;

	j = 0;
	if (flag == 1)
	{
		if (data->cmds[i].fd_input > 0)
			close(data->cmds[i].fd_input);
		if (data->cmds[i].fd_output > 2)
			close(data->cmds[i].fd_output);
	}
	if (flag == 2)
	{
		close(data->fd_pipe[0]);
		close(data->fd_pipe[1]);
		close(data->fd_tmp);
	}
	if (flag == 3)
	{
		while (j++ < data->nb_cmd)
			close_all_fds(data, 1, j);
	}
}

void	free_all_children(t_data *data)
{
	free_cmd_table(data);
	free_env(data);
	free(data);
}
