/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:48:05 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/02 15:48:56 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_get_env_val(char *var_name, t_env *envg)
{
	char	*var_tmp;
	t_env	*env_tmp;

	env_tmp = envg;
	var_tmp = NULL;
	while (env_tmp)
	{
		if (ft_strcmp(var_name, env_tmp->name) == 0)
			var_tmp = ft_strdup(env_tmp->value);
		env_tmp = env_tmp->next;
	}
	return (var_tmp);
}

int	ft_get_var_len(char *str, int quote_type)
{
	int	len;

	len = 0;
	if (str[len] == '?')
		return (len + 1);
	else if (quote_type && str[len] == quote_type)
		return (0);
	else if (!quote_type && ft_which_quote2(str[len]))
		return (-1);
	while (ft_isalnum(str[len]) || str[len] == '_')
		len++;
	return (len);
}

int	ft_get_var_val(char *str, char **var_env, int quote_type, t_data *data)
{
	char	*var_name;
	int		stop_pos;

	stop_pos = ft_get_var_len(str, quote_type);
	if (stop_pos == 0)
		return (0);
	if (stop_pos == -1)
	{
		stop_pos = 0;
		var_name = ft_strdup("");
	}
	else
		var_name = ft_substr(str, 0, stop_pos);
	if (ft_strncmp(var_name, "?", 2) == 0)
		*var_env = ft_itoa(data->mini_exit_code);
	else
		*var_env = ft_get_env_val(var_name, data->envg);
	free(var_name);
	if (!*var_env)
		*var_env = ft_strdup("");
	return (stop_pos);
}

int	ft_expand_var(char **str, int start_pos, int quote_type, t_data *data)
{
	char	*var_env;
	char	*str_replace;
	int		len;
	int		stop_pos;

	var_env = NULL;
	stop_pos = ft_get_var_val(&(*str)[start_pos], &var_env, quote_type, data);
	if (stop_pos == 0 && !var_env)
		return (1);
	stop_pos += start_pos;
	len = ft_strlen(*str) - (stop_pos - start_pos) + ft_strlen(var_env);
	str_replace = ft_calloc(sizeof(char), len + 1);
	if (!str_replace)
	{
		data->flag_pars = 1;
		data->mini_exit_code = 12;
		return (perror("Memory allocation failed"), 0);
	}
	ft_strlcat(str_replace, *str, start_pos);
	ft_strlcat(str_replace, var_env, ft_strlen(str_replace)
		+ ft_strlen(var_env) + 1);
	ft_strlcat(str_replace, &(*str)[stop_pos], len);
	stop_pos = ft_strlen(var_env);
	(free(var_env), free(*str), *str = str_replace);
	return (stop_pos);
}
