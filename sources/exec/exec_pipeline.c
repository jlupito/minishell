/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:20:11 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 15:33:39 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	wait_children_pids(t_data *data)
{
	int	i;
	int	child_status;

	i = -1;
	child_status = 0;
	(signal(SIGINT, SIG_IGN), signal(SIGQUIT, SIG_IGN));
	while (++i < data->count_childpids)
	{
		waitpid(data->pids_child[i], &child_status, 0);
		if (WIFEXITED(child_status) && WEXITSTATUS(child_status) != FAILURE)
		{
			if (WEXITSTATUS(child_status) != SUCCESS)
				data->mini_exit_code = WEXITSTATUS(child_status);
		}
		else if (WIFSIGNALED(child_status) && WTERMSIG(child_status) != SIGPIPE)
		{
			if (WTERMSIG(child_status) == SIGINT)
				write(2, "\n", 1);
			else if (WTERMSIG(child_status) == SIGQUIT)
				write(2, "Quit : core dumped\n", 19);
			data->mini_exit_code = 128 + WTERMSIG(child_status);
			return ;
		}
		data->pids_child[i] = -1;
	}
}

void	parent_process(t_data *data, int close_pipe1)
{
	if (data->pid)
	{
		data->pids_child[data->count_childpids++] = data->pid;
		close(data->fd_tmp);
		if (close_pipe1)
			close(data->fd_pipe[1]);
		data->fd_tmp = data->fd_pipe[0];
	}
}

void	child_process(t_cmd *cmds, t_data *data, int i)
{
	int	exit_builtin;

	exit_builtin = 0;
	if (!data->pid)
	{
		(signal(SIGINT, child_sigint), signal(SIGQUIT, child_sigint));
		close_next_fds(data, i);
		dup2(data->fd_tmp, STDIN_FILENO);
		close(data->fd_tmp);
		manage_pipe_fds(data, cmds[i], i);
		manage_redir_fds(data, cmds[i]);
		if (!test_builtin(cmds[i]))
			exec_function(cmds[i], data);
		else
			exit_builtin = exec_builtin(cmds[i], data);
		free_all_children(data);
		exit(exit_builtin);
	}
}

void	execution_loop(t_data *data, t_cmd *cmds, int i)
{
	int	ret;

	if (data->cmds[i].cmd_args[0])
	{
		if (i != data->nb_cmd - 1)
		{
			ret = pipe(data->fd_pipe);
			function_failed(data, ret, PIPE_FAIL, "Function pipe() failed");
		}
		data->pid = fork();
		function_failed(data, data->pid,
			FORK_FAIL, "Function fork() failed");
		child_process(cmds, data, i);
		parent_process(data, i != data->nb_cmd - 1);
		close_all_fds(data, 1, i);
	}
	else
	{
		if (data->cmds[i].heredoc > 0)
			close(data->cmds[i].heredoc);
		if (i == data->nb_cmd - 1)
			(close_all_fds(data, 1, i), close(data->fd_tmp));
	}
}

void	execute_pipeline(t_cmd *cmds, t_data *data)
{
	int		i;

	i = 0;
	data->fd_tmp = dup(STDIN_FILENO);
	function_failed(data, data->fd_tmp, MALLOC_FAIL, "Function dup() failed");
	data->count_childpids = 0;
	while (i < data->nb_cmd)
	{
		execution_loop(data, cmds, i);
		i++;
	}
	wait_children_pids(data);
	(signal(SIGINT, manage_sigint), signal(SIGQUIT, SIG_IGN));
}
