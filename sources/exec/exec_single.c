/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:30:20 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 15:14:04 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	manage_single_cmd_fds(t_cmd cmd)
{
	if (cmd.fd_input > STDIN_FILENO)
	{
		if (dup2(cmd.fd_input, STDIN_FILENO) < 0)
			return (-1);
		close(cmd.fd_input);
	}
	if (cmd.fd_output > STDOUT_FILENO)
	{
		if (dup2(cmd.fd_output, STDOUT_FILENO) < 0)
			return (-1);
		close(cmd.fd_output);
	}
	return (0);
}

void	parent_single_cmd(t_data *data, t_cmd cmd)
{
	int	child_status;

	child_status = 0;
	waitpid(cmd.id_process, &child_status, 0);
	if (WIFEXITED(child_status))
		data->mini_exit_code = WEXITSTATUS(child_status);
	else if (WIFSIGNALED(child_status))
	{
		if (WTERMSIG(child_status) == SIGINT)
			write(2, "\n", 1);
		else if (WTERMSIG(child_status) == SIGQUIT)
			write(2, "Quit : core dumped\n", 19);
		data->mini_exit_code = 128 + WTERMSIG(child_status);
	}
	if (cmd.fd_input > 0)
		close(cmd.fd_input);
	if (cmd.fd_output > 2)
		close(cmd.fd_output);
}

void	fork_and_exec_function(t_cmd cmd, t_data *data)
{
	char	**tab_env;

	cmd.id_process = fork();
	function_failed(data, cmd.id_process, FORK_FAIL, "Function fork() failed");
	if (!cmd.id_process)
	{
		(signal(SIGINT, child_sigint), signal(SIGQUIT, child_sigint));
		manage_single_cmd_fds(cmd);
		cmd.full_path = get_real_path(cmd, data);
		if (!cmd.full_path)
			(free_tabs(cmd.cmd_args), exit(COMMAND_NOT_FOUND));
		tab_env = copy_lst_in_tab2(data);
		if (!tab_env)
			(free_tabs(cmd.cmd_args),
				free(cmd.full_path), exit(data->mini_exit_code));
		if (cmd.fd_input != -1 && cmd.fd_output != -1)
			execve(cmd.full_path, cmd.cmd_args, tab_env);
		free(cmd.full_path);
		free_all_children(data);
		free_tabs(tab_env);
		exit(1);
	}
	else if (cmd.id_process > 0)
		parent_single_cmd(data, cmd);
}

void	execute_single_cmd(t_cmd cmd, t_data *data)
{
	if (test_builtin(cmd))
	{
		manage_single_cmd_fds(cmd);
		if (cmd.fd_input != -1 && cmd.fd_output != -1)
		{
			exec_builtin(cmd, data);
			dup2(STDIN_FILENO, 1);
			dup2(STDOUT_FILENO, 2);
		}
	}
	else
		fork_and_exec_function(cmd, data);
}
