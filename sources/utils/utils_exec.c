/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:08:17 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 14:20:46 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_swap_strings(char **str1, char **str2)
{
	char	*temp;

	temp = *str1;
	*str1 = *str2;
	*str2 = temp;
}

char	**ft_strdup_tab(char **tab, int size)
{
	char	**dup_tab;
	int		i;

	if (size < 0)
		return (NULL);
	dup_tab = malloc(sizeof(char *) * (size + 1));
	if (!dup_tab)
		return (NULL);
	i = -1;
	while (++i < size)
		dup_tab[i] = ft_strdup(tab[i]);
	dup_tab[size] = NULL;
	return (dup_tab);
}

void	exec_function(t_cmd cmd, t_data *data)
{
	char	**tab_env;

	cmd.full_path = get_real_path(cmd, data);
	if (!cmd.full_path)
	{
		free_tabs(cmd.cmd_args);
		perror("Command not found");
		exit(COMMAND_NOT_FOUND);
	}
	tab_env = copy_lst_in_tab2(data);
	if (!tab_env)
		(free_tabs(cmd.cmd_args), exit(data->mini_exit_code));
	if (cmd.fd_input != -1 && cmd.fd_output != -1)
		execve(cmd.full_path, cmd.cmd_args, tab_env);
	free(cmd.full_path);
	free_all_children(data);
	free_tabs(tab_env);
	exit(1);
}

char	**get_path_env(t_data *data, t_cmd cmd)
{
	char	**tab_env;
	char	**tab_paths;
	int		i;

	tab_env = copy_lst_in_tab2(data);
	if (!tab_env)
		return (NULL);
	i = 0;
	if (!existing_var_name_export("PATH", data))
		return (free_tabs(tab_env), no_such_file(data, cmd), (NULL));
	while (ft_strnstr(tab_env[i], "PATH=", 5) == SUCCESS)
		i++;
	tab_paths = ft_split(tab_env[i] + 5, ':');
	free_tabs(tab_env);
	return (tab_paths);
}

char	*get_real_path(t_cmd cmd, t_data *data)
{
	char	**tab_paths;
	char	*real_path;
	char	*path_cmd;
	int		i;

	if (access(cmd.cmd_args[0], F_OK) == SUCCESS)
		return (cmd.cmd_args[0]);
	tab_paths = get_path_env(data, cmd);
	if (!tab_paths)
		return (NULL);
	i = 0;
	while (tab_paths[i])
	{
		real_path = ft_strjoin(tab_paths[i], "/");
		path_cmd = ft_strjoin(real_path, cmd.cmd_args[0]);
		free(real_path);
		if (access(path_cmd, F_OK) == SUCCESS)
			return (free_tabs(tab_paths), path_cmd);
		(free(path_cmd), i++);
	}
	(free_tabs(tab_paths), non_existing_command(data, cmd));
	return (NULL);
}
