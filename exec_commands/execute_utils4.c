/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:59:16 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/14 15:01:23 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_io(t_lexing *cmd, t_data *data,
			int pipe_fd[2])
{
	if (data->i > 0)
	{
		dup2(data->prev_pipe, STDIN_FILENO);
		close(data->prev_pipe);
	}
	else if (cmd->infile != STDIN_FILENO)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (data->i < data->num_cmd - 1)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

void	child_exec(t_gen *gen, t_lexing *cmd, t_data *data, int pipe_fd[2])
{
	int	flag;

	flag = 0;
	if (find_red(cmd, gen))
	{
		handle_child_io(cmd, data, pipe_fd);
		free(data);
		exec_single_command(gen, cmd);
		flag = 1;
	}
	if (!flag)
	{
		close(gen->fd_stdin);
		ft_treeclear(gen->root);
		free_matrix(gen->my_env);
		free_matrix(gen->export_env);
		ft_lstclear(gen->lexed_data, 0);
		ft_lstclear(gen->cleaned_data, 1);
		free_matrix(gen->av);
		free(data);
	}
	exit(gen->exit_status);
}

int	setup_and_fork_child(t_gen *gen, t_lexing **cmds,
	t_data *data, pid_t *last_pid)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (data->i < data->num_cmd - 1 && pipe(pipe_fd) == -1)
		return (1);
	pid = fork();
	if (!ft_strncmp(cmds[data->i]->value, gen->last_cmd->value,
			ft_strlen(gen->last_cmd->value)))
		*last_pid = pid;
	if (pid == -1)
		return (1);
	if (pid == 0)
		child_exec(gen, cmds[data->i], data, pipe_fd);
	if (data->i > 0)
		close(data->prev_pipe);
	if (data->i < data->num_cmd - 1)
	{
		close(pipe_fd[1]);
		data->prev_pipe = pipe_fd[0];
	}
	return (0);
}

void	wait_for_piped_children(t_gen *gen, int num_cmds, pid_t last_pid)
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

int	prepare_piped_execution(t_gen *gen, t_tree *subroot,
	t_lexing **cmds, int *num_cmds)
{
	*num_cmds = 0;
	collect_piped_cmds(subroot, cmds, num_cmds);
	if (!check_and_execute_subtree(gen, subroot->left))
		return (0);
	if (!check_and_execute_subtree(gen, subroot->right))
		return (0);
	return (1);
}
