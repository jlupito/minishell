/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarthaud <jarthaud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:44:50 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 15:04:38 by jarthaud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_heredoc(char *prompt, t_data *data)
{
	char	*tmp;

	tmp = ft_strdup(prompt);
	free(prompt);
	prompt = ft_get_env_val(tmp + 1, data->envg);
	free(tmp);
	return (prompt);
}

void	heredoc_loop(char *prompt, t_data *data, int fd)
{
	if (prompt[0] == '$' && ft_strlen(prompt) > 1)
		prompt = expand_heredoc(prompt, data);
	write(fd, prompt, ft_strlen(prompt));
	write(fd, "\n", 1);
	free(prompt);
}

int	create_heredoc(char *delim, t_data *data)
{
	char	*prompt;
	int		pipe_hd[2];

	if (pipe(pipe_hd) == FAILURE)
		exit(PIPE_FAIL);
	while (TRUE)
	{
		prompt = readline("> ");
		if (!prompt)
		{
			ft_putstr_fd("bash: warning: here-document ", 2);
			ft_putendl_fd("delimited by end-of-file", 2);
			break ;
		}
		if (!ft_strcmp(prompt, delim))
		{
			free(prompt);
			break ;
		}
		heredoc_loop(prompt, data, pipe_hd[1]);
	}
	close(pipe_hd[1]);
	return (pipe_hd[0]);
}

void	check_heredoc(t_data *data)
{
	t_file	*tmp_in;
	int		i;

	i = 0;
	while (i < data->nb_cmd)
	{
		tmp_in = data->cmds[i].file_in;
		while (tmp_in)
		{
			if (tmp_in->token_type == HERE_DOC)
				data->cmds[i].heredoc = create_heredoc(tmp_in->file_name, data);
			tmp_in = tmp_in->next;
		}
		i++;
	}
}
