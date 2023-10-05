/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_launching.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alibourb <alibourb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:27:05 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 11:54:24 by alibourb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_env	*save_env(t_data *data, char **env)
{
	int		i;
	int		env_len;
	t_env	*env_to_save;

	env_len = 0;
	while (env[env_len])
		env_len++;
	env_to_save = NULL;
	i = 0;
	while (i < env_len)
	{
		if (i == 0)
			env_to_save = ft_new_env(data, env[i]);
		else
		{
			env_to_save = ft_add_back_env(&env_to_save,
					ft_new_env(data, env[i]));
			if (!env_to_save)
				return (free_env(data), NULL);
		}
		i++;
	}
	return (env_to_save);
}

int	ft_lstsize_env(t_env *envg)
{
	unsigned int	nbelements;
	t_env			*temp;

	nbelements = 0;
	temp = envg;
	while (temp)
	{
		temp = temp->next;
		nbelements++;
	}
	return (nbelements);
}

void	ft_init_mini(t_data *data, char **env)
{
	data->mini_exit_code = 0;
	data->cmds = NULL;
	data->line_lexered = NULL;
	data->env_export = NULL;
	if (env)
	{
		data->envg = save_env(data, env);
		if (data->envg)
			data->env_export = copy_lst_in_tab(data);
	}
}

int	setup_lauching(t_data **data, int ac, char **av, char **env)
{
	*data = NULL;
	if (ac != 1 || av[1])
		exit(INIT_FAIL);
	*data = malloc(sizeof(t_data) * 1);
	if (!*data)
	{
		perror("Memory allocation failed");
		exit(ENOMEM);
	}
	ft_init_mini(*data, env);
	return (SUCCESS);
}

void	executor(t_data *data)
{
	(signal(SIGQUIT, SIG_IGN), signal(SIGINT, SIG_IGN));
	if (data->nb_cmd > 1)
		execute_pipeline(data->cmds, data);
	else
	{
		if (!data->cmds[0].cmd_args[0])
		{
			if (data->cmds[0].heredoc > 0)
				close(data->cmds[0].heredoc);
			return ;
		}
		execute_single_cmd(*(data->cmds), data);
	}
	(signal(SIGINT, manage_sigint), signal(SIGQUIT, SIG_IGN));
}
