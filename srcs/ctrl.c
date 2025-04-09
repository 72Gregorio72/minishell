/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 09:24:48 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/09 11:59:10 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrl_c(int new_line)
{
	if (new_line)
		write(STDOUT_FILENO, "\n", 1);
	if (RL_ISSTATE(RL_STATE_READCMD))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ctrl_d(t_gen *gen)
{
	(void)gen;
	printf("exit\n");
	free_matrix(gen->my_env);
	free_matrix(gen->export_env);
	rl_clear_history();
	exit(0);
}

void	ctrl_backslash(void)
{
	printf("Quit (core dumped)\n");
	if (RL_ISSTATE(RL_STATE_READCMD))
		ctrl_c(0);
	g_sig_received = CTRL_BACK;
}
