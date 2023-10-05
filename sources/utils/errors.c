/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:06:16 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 14:48:08 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_message(char *msg, char *cmd, int code, t_data *data)
{
	char	*cmd_name;

	if (cmd)
		cmd_name = ft_strjoin("minishort: ", cmd);
	else
		cmd_name = ft_strjoin("minishort: ", "unknown");
	ft_putstr_fd(cmd_name, 2);
	if (msg)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(msg, 2);
	}
	ft_putstr_fd("\n", 2);
	data->mini_exit_code = code;
	free(cmd_name);
}

void	non_existing_command(t_data *data, t_cmd cmd)
{
	ft_putstr_fd("minishort: ", STDERR_FILENO);
	ft_putstr_fd(cmd.cmd_args[0], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	free_env(data);
	if (data->cmds)
		free_cmd_table(data);
	free (data);
	exit(COMMAND_NOT_FOUND);
}

void	no_such_file(t_data *data, t_cmd cmd)
{
	data->mini_exit_code = NO_FILE_DIR_PATH;
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd.cmd_args[0], STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	free_env(data);
	if (data->cmds)
		free_cmd_table(data);
	free (data);
	exit(NO_FILE_DIR_PATH);
}

int	ft_check_particular(t_data *data)
{
	size_t	i;
	char	*tmp;

	i = 0;
	tmp = data->line_lexered->str;
	if (ft_strlen(tmp) == 1 && tmp[i] == ':')
		return (data->mini_exit_code = 0, FAILURE);
	if (ft_strlen(tmp) == 1 && tmp[i] == '!')
		return (data->mini_exit_code = 1, FAILURE);
	if (tmp[i] == '/')
	{
		while (tmp[i])
		{
			if (tmp[i] == '/')
			{
				if (i == ft_strlen(tmp) - 1)
					return (error_message("Is a directory", tmp,
							126, data), -1);
				i++;
			}
			else
				break ;
		}
	}
	return (0);
}

void	function_failed(t_data *data, int ret, int type_fail,
	char *error_message)
{
	if (ret == FAILURE)
	{
		perror(error_message);
		if (data->fd_tmp)
			close(data->fd_tmp);
		data->mini_exit_code = type_fail;
		return ;
	}
}
