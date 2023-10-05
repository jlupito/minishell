/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:17:11 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 16:34:41 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_env(t_data *data)
{
	t_env	*tmp;

	if (!data->envg)
		return ;
	tmp = data->envg;
	while (tmp)
	{
		tmp = tmp->next;
		free(data->envg->value);
		free(data->envg->name);
		free(data->envg);
		data->envg = tmp;
	}
	free_tabs(data->env_export);
}

t_env	*ft_new_env(t_data *data, char *str)
{
	t_env	*new;
	char	**tmp;
	char	*shlvl;

	new = malloc(sizeof(t_env));
	if (!new)
	{
		(perror("Memory allocation failed"), data->mini_exit_code = 12);
		return (NULL);
	}
	tmp = ft_split_equals(str);
	new->name = NULL;
	new->value = NULL;
	new->name = ft_strdup(tmp[0]);
	if (!ft_strcmp(tmp[0], "SHLVL"))
	{
		shlvl = ft_itoa(ft_atoi(tmp[1]) + 1);
		new->value = shlvl;
	}
	else
		new->value = ft_strdup(tmp[1]);
	free_tabs(tmp);
	new->next = (NULL);
	return (new);
}

t_env	*ft_last_env(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_env	*ft_add_back_env(t_env**lst, t_env *new)
{
	t_env	*add_last;

	if (!new)
		return (NULL);
	else
	{
		add_last = ft_last_env(*lst);
		add_last->next = new;
	}
	return (*lst);
}
