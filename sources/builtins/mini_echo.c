/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alibourb <alibourb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:02:04 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 17:39:07 by alibourb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_echo(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		ft_putstr_fd(str[i++], STDOUT_FILENO);
		if (str[i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
}

int	mini_echo(t_data *data, t_cmd cmd)
{
	char	**words;
	int		flag;

	flag = 0;
	words = cmd.cmd_args;
	if (!cmd.cmd_args[1])
	{
		flag = 1;
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	else if (cmd.cmd_args[1] && ft_strcmp(cmd.cmd_args[1], "-n") == SUCCESS)
	{
		flag = 1;
		print_echo(words + 2);
	}
	else
		print_echo(words + 1);
	if (!flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	data->mini_exit_code = SUCCESS;
	return (SUCCESS);
}
