/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_display.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alibourb <alibourb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 16:07:59 by jarthaud          #+#    #+#             */
/*   Updated: 2023/10/03 17:14:27 by alibourb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	display_token(t_lexer **lexer)
{
	size_t	i;
	t_lexer	*current;

	i = 0;
	current = *lexer;
	printf("\n\n---------------TABLEAU DE TOKENS-------------\n");
	printf("| %-5s | %-25s | %-5s |\n", "#", "Token Data", "Type");
	printf("---------------------------------------------\n");
	while (current)
	{
		printf("| %-5zu | %-25s | %-5d |\n", i,
			current->str, current->token_type);
		current = current->next;
		i++;
	}
	printf("---------------------------------------------\n\n\n");
}

// void	print_block_cmd(t_data *data)
// {
// 	int			i;
// 	int			j;
// 	t_data		*tmp;

// 	tmp = data;
// 	i = 0;
// 	while (i < tmp->nb_cmd)
// 	{
// 		printf(" --------- cmd %d--------\n", i);
// 		j = 0;
// 		while (tmp->cmds[i].cmd_args[j])
// 		{
// 			printf("argv %d : %s\n", j, tmp->cmds[i].cmd_args[j]);
// 			j++;
// 		}
// 		printf("path : %s\n", tmp->cmds[i].full_path);
// 		printf("fd_input : %d\n", tmp->cmds[i].fd_input);
// 		printf("fd_output : %d\n", tmp->cmds[i].fd_output);
// 		while (tmp->cmds[i].file_in)
// 		{
// 			printf("file_name in : %s\n", tmp->cmds[i].file_in->file_name);
// 			printf("token type : %d\n", tmp->cmds[i].file_in->token_type);
// 			tmp->cmds[i].file_in = tmp->cmds[i].file_in->next;
// 		}
// 		while (tmp->cmds[i].file_out)
// 		{
// 			printf("file_name out : %s\n", tmp->cmds[i].file_out->file_name);
// 			printf("token type: %d\n", tmp->cmds[i].file_out->token_type);
// 			tmp->cmds[i].file_out = tmp->cmds[i].file_out->next;
// 		}
// 		i++;
// 	}
// }
