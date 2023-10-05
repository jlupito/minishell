/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alibourb <alibourb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:27:17 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 18:22:42 by alibourb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	child_sigint(int signal)
{
	if (signal == SIGINT)
	{
		(void)signal;
		g_signal_status = 130;
		write(2, "\n", 1);
	}
	else if (signal == SIGQUIT)
	{
		(void)signal;
		g_signal_status = 131;
		write(2, "Quit : core dumped\n", 19);
	}
}

void	manage_sigint(int signum)
{
	if (signum == SIGINT)
	{
		g_signal_status = 130;
		write(2, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signals_monitoring(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, manage_sigint);
	signal(SIGQUIT, SIG_IGN);
}
