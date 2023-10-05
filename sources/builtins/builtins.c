/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:01:02 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 15:25:02 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	test_builtin(t_cmd cmd)
{
	char	*name;

	name = cmd.cmd_args[0];
	if (!ft_strcmp(name, "echo") || !ft_strcmp(name, "cd")
		|| !ft_strcmp(name, "pwd") || !ft_strcmp(name, "export")
		|| !ft_strcmp(name, "unset") || !ft_strcmp(name, "env")
		|| !ft_strcmp(name, "exit"))
		return (TRUE);
	return (FALSE);
}

int	exec_builtin(t_cmd cmd, t_data *data)
{
	const char	*name;
	int			exit_builtin;

	exit_builtin = SUCCESS;
	name = cmd.cmd_args[0];
	if (!ft_strcmp(name, "echo"))
		exit_builtin = mini_echo(data, cmd);
	if (!ft_strcmp(name, "cd"))
		exit_builtin = mini_cd(cmd, data);
	if (!ft_strcmp(name, "pwd"))
		exit_builtin = mini_pwd(data);
	if (!ft_strcmp(name, "export"))
		exit_builtin = mini_export(cmd, data);
	if (!ft_strcmp(name, "unset"))
		exit_builtin = mini_unset(cmd, data);
	if (!ft_strcmp(name, "env"))
		exit_builtin = mini_env(data);
	if (!ft_strcmp(name, "exit"))
		exit_builtin = mini_exit(cmd, data);
	return (exit_builtin);
}
