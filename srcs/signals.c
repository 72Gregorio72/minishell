<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:27:09 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/09 12:17:26 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig_received = 0;

void	handler(int sign, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (sign == SIGINT)
	{
		g_sig_received = CTRL_C;
		ctrl_c(1);
	}
	if (sign == SIGQUIT)
		ctrl_backslash();
}

int	checks(char *line, t_gen *gen)
{
	if (!line)
		ctrl_d(gen);
	else if (line[0] == '\0')
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		return (1);
	}
	if (g_sig_received == CTRL_C)
	{
		g_sig_received = 0;
		gen->exit_status = 130;
	}
	if (g_sig_received == CTRL_BACK)
	{
		g_sig_received = 0;
		gen->exit_status = 131;
	}
	return (0);
}

int	set_signals(struct sigaction sa)
{
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (0);
	if (isatty(STDIN_FILENO))
		signal(SIGQUIT, SIG_IGN);
	else if (sigaction(SIGQUIT, &sa, NULL) == -1)
		return (0);
	return (1);
}
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:27:09 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/09 12:17:26 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig_received = 0;

void	handler(int sign, siginfo_t *info, void *context)
{
	(void)context;
	(void)info;
	if (sign == SIGINT)
	{
		g_sig_received = CTRL_C;
		ctrl_c(1);
	}
	if (sign == SIGQUIT)
		ctrl_backslash();
}

int	checks(char *line, t_gen *gen)
{
	if (!line)
		ctrl_d(gen);
	else if (line[0] == '\0')
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		return (1);
	}
	if (g_sig_received == CTRL_C)
	{
		g_sig_received = 0;
		gen->exit_status = 130;
	}
	if (g_sig_received == CTRL_BACK)
	{
		g_sig_received = 0;
		gen->exit_status = 131;
	}
	return (0);
}

int	set_signals(struct sigaction sa)
{
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (0);
	if (isatty(STDIN_FILENO))
		signal(SIGQUIT, SIG_IGN);
	else if (sigaction(SIGQUIT, &sa, NULL) == -1)
		return (0);
	return (1);
}
>>>>>>> origin/vcastald
