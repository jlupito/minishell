/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:01:26 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 15:17:08 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	get_new_path_direc(char *cmd_path, char **add_new_path, char *old)
{
	char	*tmp;

	tmp = ft_strjoin(cmd_path, "/");
	*add_new_path = ft_strjoin(tmp, old);
	free(tmp);
}

void	get_new_direc(t_data *data, t_cmd cmd, char *old)
{
	char	*cmd_path;
	char	*new_path;

	if (old && !ft_strcmp(cmd.cmd_args[1], old))
		return (free(old));
	cmd_path = cmd.cmd_args[1];
	if ((!ft_strcmp(cmd_path, "//")) || (!ft_strcmp(cmd_path, "/")))
		new_path = ft_strdup("/");
	else
	{
		if (!access(cmd_path, F_OK))
			new_path = ft_strdup(cmd_path);
		else
		{
			if (old)
				get_new_path_direc(cmd_path, &new_path, old);
			else
				return (error_message("getcwd: No such file or directory",
						"chdir: error retrieving current directory", 1, data));
		}
	}
	if (!new_path || chdir(new_path) == FAILURE)
		return (free(new_path), cd_error(data, old, cmd.cmd_args[1], 1));
	(free(new_path), new_path = getcwd(NULL, 0));
	(update_pwd(old, new_path, data), data->mini_exit_code = SUCCESS);
}

void	update_pwd(char *old_path, char *new_path, t_data *data)
{
	t_env	*current;

	current = data->envg;
	if (!existing_var_name("OLDPWD", data))
		add_var("OLDPWD", NULL, data);
	while (current != NULL)
	{
		if (!ft_strncmp(current->name, "PWD", 3) && new_path)
		{
			free(current->value);
			current->value = ft_strdup(new_path);
		}
		if (!ft_strncmp(current->name, "OLDPWD", 6) && old_path)
		{
			if (current->value)
				free(current->value);
			current->value = ft_strdup(old_path);
		}
		current = current->next;
	}
	(free(old_path), free(new_path));
}

void	cd_error(t_data *data, char *old_path, char *cmd, int flag)
{
	ft_putstr_fd("minishort: cd: ", STDERR_FILENO);
	if (flag == 1 && ft_strcmp(cmd, "HOME"))
	{
		if (!existing_var_name("OLDPWD", data))
			ft_putstr_fd("OLDPWD not set\n", STDERR_FILENO);
		else
		{
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
	}
	else if (flag == 2 || (flag == 1 && !ft_strcmp(cmd, "HOME")))
		ft_putstr_fd("Home not set\n", STDERR_FILENO);
	free(old_path);
	(data->mini_exit_code = 1);
	return ;
}

int	mini_cd(t_cmd cmd, t_data *data)
{
	char	*new;
	char	*old;

	old = getcwd(NULL, 0);
	if (!cmd.cmd_args[1] || !ft_strcmp(cmd.cmd_args[1], "~"))
	{
		new = ft_get_env_val("HOME", data->envg);
		if (!new || chdir(new) == FAILURE)
			return (free(new),
				cd_error(data, old, cmd.cmd_args[1], 2), FAILURE);
	}
	else if (!ft_strcmp(cmd.cmd_args[1], "-"))
	{
		new = ft_get_env_val("OLDPWD", data->envg);
		if (!new || chdir(new) == FAILURE)
			return (free(new),
				cd_error(data, old, cmd.cmd_args[1], 1), FAILURE);
	}
	else
		return (get_new_direc(data, cmd, old), data->mini_exit_code);
	free(new);
	new = getcwd(NULL, 0);
	update_pwd(old, new, data);
	data->mini_exit_code = SUCCESS;
	return (SUCCESS);
}
