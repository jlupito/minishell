/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lists.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:08:24 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 16:56:02 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	array_len(char **tab)
{
	int		i;

	i = 0;
	if (!tab)
		return (i);
	while (tab[i])
		i++;
	return (i);
}

int	process_copy_lst(t_env *current, char **env_tab)
{
	int	i;

	i = 0;
	while (current != NULL)
	{
		env_tab[i] = malloc(sizeof(char) * (ft_strlen(current->name)
					+ ft_strlen(current->value) + 4));
		if (!env_tab[i])
			return (free_tabs(env_tab), MALLOC_FAIL);
		env_tab[i][0] = '\0';
		ft_strcpy(env_tab[i], current->name);
		ft_strcat(env_tab[i], "=\"");
		ft_strcat(env_tab[i], current->value);
		ft_strcat(env_tab[i], "\"");
		current = current->next;
		i++;
	}
	env_tab[i] = NULL;
	return (SUCCESS);
}

char	**copy_lst_in_tab(t_data *data)
{
	char	**env_tab;
	int		len_tab;
	t_env	*current;

	len_tab = ft_lstsize_env(data->envg);
	env_tab = malloc(sizeof(char *) * (len_tab + 1));
	if (!env_tab)
	{
		data->mini_exit_code = MALLOC_FAIL;
		return (perror("Memory allocation failed"), NULL);
	}
	current = data->envg;
	if (process_copy_lst(current, env_tab))
		return (perror("Allocation memory failed"), NULL);
	return (env_tab);
}

int	process_copy_lst2(t_env *current, char **env_tab)
{
	int	i;

	i = 0;
	while (current != NULL)
	{
		env_tab[i] = malloc(sizeof(char) * (ft_strlen(current->name)
					+ ft_strlen(current->value) + 2));
		if (!env_tab[i])
			return (free_tabs(env_tab), MALLOC_FAIL);
		env_tab[i][0] = '\0';
		ft_strcpy(env_tab[i], current->name);
		ft_strcat(env_tab[i], "=");
		ft_strcat(env_tab[i], current->value);
		current = current->next;
		i++;
	}
	env_tab[i] = NULL;
	return (SUCCESS);
}

char	**copy_lst_in_tab2(t_data *data)
{
	char	**env_tab;
	int		len_tab;
	t_env	*current;

	len_tab = ft_lstsize_env(data->envg);
	env_tab = malloc(sizeof(char *) * (len_tab + 1));
	if (!env_tab)
	{
		data->mini_exit_code = MALLOC_FAIL;
		return (perror("Allocation memory failed"), NULL);
	}
	current = data->envg;
	if (process_copy_lst2(current, env_tab))
	{
		data->mini_exit_code = MALLOC_FAIL;
		return (perror("Allocation memory failed"), NULL);
	}
	return (env_tab);
}
