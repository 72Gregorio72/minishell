/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/26 12:29:46 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_process(t_gen *gen, int num_cmds, pid_t last_pid)
{
	int		i;
	int		status;
	pid_t	pid;

	i = 0;
	while (i < num_cmds)
	{
		pid = wait(&status);
		if (pid == last_pid && WIFEXITED(status))
			gen->exit_status = WEXITSTATUS(status);
		i++;
	}
}

int    check_error(t_piped *piped, t_gen *gen)
{
    if (piped->i < piped->num_cmds - 1 && pipe(piped->pipe_fd) == -1)
    {
        ft_putstr_fd("pipe error\n", 2);
        gen->exit_status = 1;
        return (1);
    }
    return (0);
}

int	check_fork(pid_t pid, t_gen *gen)
{
	if (pid == -1)
	{
		ft_putstr_fd("fork error\n", 2);
		gen->exit_status = 1;
		return (1);
	}
	return (0);
}

void	close_pipes(t_piped *piped)
{
	if (piped->i > 0)
		close(piped->prev_pipe);
	if (piped->i < piped->num_cmds - 1)
	{
		close(piped->pipe_fd[1]);
		piped->prev_pipe = piped->pipe_fd[0];
	}
}

void	exec_piped_commands(t_gen *gen, t_tree *subroot)
{
	t_piped		piped;
	pid_t		pid;
	t_lexing	*last_cmd;
	pid_t		last_pid;

	piped.num_cmds = 0;
	piped.prev_pipe = -1;
	collect_piped_cmds(subroot, piped.cmds, &piped.num_cmds, gen);
	last_cmd = piped.cmds[piped.num_cmds - 1];
	piped.i = -1;
	while (++piped.i < piped.num_cmds)
	{
		if (check_error(&piped, gen))
   			return ;
		pid = fork();
		if (check_fork(pid, gen))
			return ;
		if (!ft_strncmp(piped.cmds[piped.i]->value,
				last_cmd->value, ft_strlen(last_cmd->value)))
			last_pid = pid;
		if (pid == 0)
			son_piped(gen, &piped);
		close_pipes(&piped);
	}
	wait_process(gen, piped.num_cmds, last_pid);
}
