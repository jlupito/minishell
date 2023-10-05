/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:02:16 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 15:02:19 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	mini_env(t_data *data)
{
	t_env	*temp;

	temp = data->envg;
	if (!temp)
	{
		ft_putstr_fd("No environment variables set.\n", STDOUT_FILENO);
		data->mini_exit_code = NO_ENV_SET;
		return (NO_ENV_SET);
	}
	while (temp)
	{
		if (temp->value)
		{
			ft_putstr_fd(temp->name, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(temp->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		temp = temp->next;
	}
	data->mini_exit_code = SUCCESS;
	return (SUCCESS);
}
