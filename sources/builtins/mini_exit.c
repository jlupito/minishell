/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:02:26 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 15:06:03 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_is_valid_numb(const char *nptr)
{
	int	i;

	i = 0;
	if (nptr[0] == '-' || nptr[0] == '+')
	{
		i++;
		if (!(nptr[1] >= '0' && nptr[i] <= '9'))
			return (FALSE);
	}
	while (nptr[i])
	{
		if (nptr[i] >= '0' && nptr[i] <= '9')
			i++;
		else
			return (FALSE);
	}
	return (TRUE);
}

void	mini_exit_args(char **cmd_args)
{
	if (cmd_args[2] && ft_is_valid_numb(cmd_args[1]))
		ft_putstr_fd("minishort: exit: too many arguments\n", STDERR_FILENO);
	else if (cmd_args[2] && !ft_is_valid_numb(cmd_args[1]))
	{
		ft_putstr_fd("minishort: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd_args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	}
}

void	mini_exit_code(t_cmd cmd, t_data *data)
{
	int	temp_exit;

	temp_exit = 0;
	if (!cmd.cmd_args[2] && ft_is_valid_numb(cmd.cmd_args[1]))
	{
		temp_exit = ft_atoi(cmd.cmd_args[1]);
		ft_putendl_fd("exit", STDOUT_FILENO);
		free_cmd_table(data);
		free_env(data);
		free(data);
		exit(temp_exit);
	}
	else if (!cmd.cmd_args[2] && !ft_is_valid_numb(cmd.cmd_args[1]))
	{
		ft_putstr_fd("minishort: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd.cmd_args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	}
	if (cmd.cmd_args[2])
		mini_exit_args(cmd.cmd_args);
	free_cmd_table(data);
	free_env(data);
	free(data);
	exit(TOO_MANY_ARG);
}

int	mini_exit(t_cmd cmd, t_data *data)
{
	if (!cmd.cmd_args[1])
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		free_cmd_table(data);
		free_env(data);
		free(data);
		exit(EXIT_SUCCESS);
	}
	else if (cmd.cmd_args[1])
		mini_exit_code(cmd, data);
	return (SUCCESS);
}
