/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 09:24:48 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/06 11:27:02 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrl_c(int new_line)
{
	if (new_line)
		write(2, "\n", 1);
	if (RL_ISSTATE(RL_STATE_READCMD))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ctrl_d(t_gen *gen)
{
	printf("exit\n");
	free_matrix(gen->my_env);
	free_matrix(gen->export_env);
	rl_clear_history();
	exit(gen->exit_status);
}

void	ctrl_backslash(void)
{
	ft_putendl_fd("Quit (core dumped)", 2);
	if (RL_ISSTATE(RL_STATE_READCMD))
		ctrl_c(0);
	g_sig_received = CTRL_BACK;
}
