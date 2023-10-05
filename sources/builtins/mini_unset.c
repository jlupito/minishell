/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:14:16 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 15:16:38 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	unset_var_env(char *cmd_var, t_data *data)
{
	t_env	*prev;
	t_env	*current;

	prev = NULL;
	current = data->envg;
	while (current)
	{
		if (!ft_strcmp(cmd_var, current->name))
		{
			if (!prev)
				data->envg = current->next;
			else
				prev->next = current->next;
			free(current->value);
			free(current->name);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	unset_var_export(char *cmd_var, t_data *data)
{
	char	**new_export;
	char	**tmp_export_name;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_export = malloc(sizeof(char *) * array_len(data->env_export));
	if (!new_export)
	{
		(perror("Memory allocation failed"), data->mini_exit_code = 12);
		return ;
	}
	while (i < array_len(data->env_export))
	{
		tmp_export_name = ft_split_equals(data->env_export[i]);
		if (ft_strcmp(tmp_export_name[0], cmd_var))
		{
			new_export[j] = ft_strdup(data->env_export[i]);
			j++;
		}
		(free_tabs(tmp_export_name), i++);
	}
	new_export[j] = NULL;
	(free_tabs(data->env_export), data->env_export = new_export);
}

int	existing_var_name(const char *var_arg, t_data *data)
{
	t_env	*current;

	current = data->envg;
	if (!var_arg || !data->envg)
		return (0);
	while (current)
	{
		if (!ft_strcmp(var_arg, current->name))
			return (TRUE);
		current = current->next;
	}
	return (FALSE);
}

int	existing_var_name_export(const char *var_arg, t_data *data)
{
	int		i;
	char	**tmp_exp_env;

	i = 0;
	while (i < array_len(data->env_export))
	{
		tmp_exp_env = ft_split_equals(data->env_export[i]);
		if (!ft_strcmp(var_arg, tmp_exp_env[0]))
			return (free_tabs(tmp_exp_env), TRUE);
		free_tabs(tmp_exp_env);
		i++;
	}
	return (FALSE);
}

int	mini_unset(t_cmd cmd, t_data *data)
{
	int		i;

	i = 1;
	if (!cmd.cmd_args[1])
	{
		data->mini_exit_code = WRONG_ARG;
		return (WRONG_ARG);
	}
	while (cmd.cmd_args[i])
	{
		if (existing_var_name(cmd.cmd_args[i], data))
			unset_var_env(cmd.cmd_args[i], data);
		if (existing_var_name_export(cmd.cmd_args[i], data))
			unset_var_export(cmd.cmd_args[i], data);
		i++;
	}
	return (data->mini_exit_code);
}
