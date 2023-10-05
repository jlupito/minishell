/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alibourb <alibourb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:08:31 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 17:48:48 by alibourb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal_status = 0;

int	prompt_loop(char *prompt, t_data *data)
{
	add_history(prompt);
	if (!ft_lexer_parse(data, prompt))
	{
		if (!ft_expand(data) || !ft_parser(data))
		{
			(free_lexer(data->line_lexered));
			return (1);
		}
		executor(data);
		free_cmd_table(data);
	}
	else
		free_lexer(data->line_lexered);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char	*prompt;
	t_data	*data;

	if (setup_lauching(&data, ac, av, env) != SUCCESS)
		exit(INIT_FAIL);
	while (TRUE)
	{
		signals_monitoring();
		prompt = readline("minishort$> ");
		if (!prompt)
			break ;
		if (g_signal_status)
		{
			data->mini_exit_code = g_signal_status;
			g_signal_status = 0;
		}
		if (ft_strlen(prompt))
		{
			if (prompt_loop(prompt, data))
				continue ;
		}
	}
	(free_env(data), free(data));
	return (SUCCESS);
}
