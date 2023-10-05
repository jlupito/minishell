/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export_print.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:08:36 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 12:30:21 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	replace_var_in_export(char *var_name, char *var_value, t_data *data)
{
	int		i;
	char	**tmp_exp_env;
	int		len;

	i = 0;
	len = array_len(data->env_export);
	while (i < len)
	{
		tmp_exp_env = ft_split_equals(data->env_export[i]);
		if (!ft_strcmp(var_name, tmp_exp_env[0]))
		{
			unset_var_export(tmp_exp_env[0], data);
			add_var_in_export(var_name, var_value, data, 1);
		}
		free_tabs(tmp_exp_env);
		i++;
	}
}

int	add_var_in_export(char *var_name, char *var_value, t_data *data,
			int flag)
{
	char	**new_env_export;
	char	*tmp;
	int		i;

	new_env_export = malloc(sizeof(char *) * (array_len(data->env_export) + 2));
	if (!new_env_export)
	{
		data->mini_exit_code = MALLOC_FAIL;
		return (perror("Memory allocation failed"), MALLOC_FAIL);
	}
	i = -1;
	while (++i < array_len(data->env_export))
		new_env_export[i] = ft_strdup(data->env_export[i]);
	if (flag == 1)
		tmp = ft_add_new_value(var_name, var_value, data);
	else
		tmp = ft_strdup(var_name);
	new_env_export[i] = ft_strdup(tmp);
	new_env_export[i + 1] = NULL;
	(free(tmp), free_tabs(data->env_export));
	data->env_export = new_env_export;
	data->mini_exit_code = SUCCESS;
	return (SUCCESS);
}

char	**ft_sort_tab(t_data *data, int size, char **tab)
{
	int		j;
	char	**sorted_tab;

	sorted_tab = ft_strdup_tab(tab, size);
	if (!sorted_tab)
	{
		data->mini_exit_code = MALLOC_FAIL;
		perror("Memory allocation failed");
		return (NULL);
	}
	j = 0;
	while (j < size - 1)
	{
		if (ft_strcmp(sorted_tab[j], sorted_tab[j + 1]) > 0)
		{
			ft_swap_strings(&sorted_tab[j], &sorted_tab[j + 1]);
			j = 0;
		}
		else
			j++;
	}
	return (sorted_tab);
}

int	sort_and_print_var_export(t_cmd cmd, t_data *data)
{
	int		i;
	int		size_tab;
	char	**env_tab_sorted;

	i = 0;
	size_tab = array_len(data->env_export);
	env_tab_sorted = ft_sort_tab(data, size_tab, data->env_export);
	if (!env_tab_sorted)
		return (data->mini_exit_code);
	while (i < size_tab)
	{
		ft_putstr_fd("export ", cmd.fd_output);
		ft_putstr_fd(env_tab_sorted[i], cmd.fd_output);
		ft_putstr_fd("\n", cmd.fd_output);
		i++;
	}
	free_tabs(env_tab_sorted);
	data->mini_exit_code = SUCCESS;
	return (SUCCESS);
}
