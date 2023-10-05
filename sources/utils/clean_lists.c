/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_lists.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:05:57 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/04 12:30:11 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_tabs(char **tab)
{
	size_t	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			if (tab[i])
				free(tab[i]);
			i++;
		}
		free(tab);
	}
}

void	free_cmd_iofile(t_file *file)
{
	t_file	*tmp;

	if (!file)
		return ;
	tmp = file;
	while (tmp)
	{
		tmp = tmp->next;
		free(file->file_name);
		free(file);
		file = tmp;
	}
}

void	free_lexer(t_lexer *lexer)
{
	t_lexer	*tmp;

	if (!lexer)
		return ;
	tmp = lexer;
	while (tmp)
	{
		tmp = tmp->next;
		free(lexer->str);
		free(lexer);
		lexer = tmp;
	}
}

void	free_cmd_table(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_cmd)
	{
		if (data->cmds[i].file_in)
			free_cmd_iofile(data->cmds[i].file_in);
		if (data->cmds[i].file_out)
			free_cmd_iofile(data->cmds[i].file_out);
		if (data->cmds[i].cmd_args)
			free_tabs(data->cmds[i].cmd_args);
		if (data->cmds[i].full_path)
			free(data->cmds[i].full_path);
		i++;
	}
	if (data->cmds)
		free(data->cmds);
}

void	free_cmd_table_child(t_cmd *cmds)
{
	if (cmds->file_in)
		free_cmd_iofile(cmds->file_in);
	if (cmds->file_out)
		free_cmd_iofile(cmds->file_out);
	if (cmds->cmd_args)
		free_tabs(cmds->cmd_args);
	if (cmds->full_path)
		free(cmds->full_path);
	if (cmds)
		free(cmds);
}
