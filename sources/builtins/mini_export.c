/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:10:06 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 13:41:12 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_var_name(t_data *data, char *cmd_var)
{
	if (!ft_isalpha(cmd_var[0]) && cmd_var[0] != '_')
	{
		ft_putstr_fd("minishort: export: '", STDERR_FILENO);
		ft_putstr_fd(cmd_var, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		data->mini_exit_code = WRONG_ARG;
		return (FALSE);
	}
	cmd_var++;
	while (*cmd_var)
	{
		if (!ft_isalnum(*cmd_var) && *cmd_var != '_')
		{
			ft_putstr_fd("minishort: export: '", STDERR_FILENO);
			ft_putstr_fd(cmd_var, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			data->mini_exit_code = WRONG_ARG;
			return (FALSE);
		}
		cmd_var++;
	}
	return (TRUE);
}

char	*ft_add_new_value(char *name, char *val, t_data *data)
{
	char	*new;

	if (val && val[0] == '"' && val[ft_strlen(val)] == '"')
		return (val);
	else
	{
		new = malloc((sizeof(char) * ft_strlen(val) + ft_strlen(name) + 5));
		if (!new)
			return (error_message(NULL, "Malloc failed", 12, data), NULL);
		if (name)
		{
			new = ft_strcpy(new, name);
			new = ft_strcat(new, "=\"");
			if (val)
				new = ft_strcat(new, val);
			return (new = ft_strcat(new, "\""));
		}
		else
		{
			new = ft_strcpy(new, "=\"");
			if (val)
				new = ft_strcat(new, val);
			return (new = ft_strcat(new, "\""));
		}
	}
}

void	add_var(char *var_name, char *var_value, t_data *data)
{
	t_env	*new_var;
	t_env	*last;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
	{
		free(var_value);
		free(var_name);
		data->mini_exit_code = MALLOC_FAIL;
		perror("Memory allocation failed");
		return ;
	}
	new_var->name = ft_strdup(var_name);
	new_var->value = ft_strdup(var_value);
	new_var->next = NULL;
	last = data->envg;
	if (!last)
		data->envg = new_var;
	else
	{
		while (last->next)
			last = last->next;
		last->next = new_var;
	}
	data->mini_exit_code = SUCCESS;
}

void	create_new_var(char *args, t_data *data)
{
	t_env	*current;
	char	**var_split;

	var_split = ft_split_equals(args);
	current = data->envg;
	while (current)
	{
		if (ft_strcmp(var_split[0], current->name) == 0)
		{
			(free(current->value), current->value = ft_strdup(var_split[1]));
			break ;
		}
		current = current->next;
	}
	if (is_valid_var_name(data, var_split[0]))
	{
		if (!current)
			add_var(var_split[0], var_split[1], data);
		if (!existing_var_name_export(var_split[0], data))
			add_var_in_export(var_split[0], var_split[1], data, 1);
		else if (existing_var_name_export(var_split[0], data))
			(unset_var_export(var_split[0], data),
				add_var_in_export(var_split[0], var_split[1], data, 1));
	}
	free_tabs(var_split);
}

int	mini_export(t_cmd cmd, t_data *data)
{
	int		i;

	if ((!cmd.cmd_args[1]))
		return (sort_and_print_var_export(cmd, data));
	i = 1;
	while (cmd.cmd_args[i])
	{
		if (ft_strchr(cmd.cmd_args[i], '=') && cmd.cmd_args[i][0] != '=')
			create_new_var(cmd.cmd_args[i], data);
		else
		{
			if (is_valid_var_name(data, cmd.cmd_args[i]))
			{
				if (!existing_var_name_export(cmd.cmd_args[i], data))
					add_var_in_export(cmd.cmd_args[i], NULL, data, 0);
			}
		}
		i++;
	}
	data->mini_exit_code = SUCCESS;
	return (SUCCESS);
}
