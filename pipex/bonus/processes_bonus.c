/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 09:58:20 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/06 11:23:47 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_all_pipes(t_data_bonus *data)
{
	int	i;

	i = 0;
	while (i < (data->cmd_num - 1) * 2)
	{
		if (data->fd[i] != -1)
			close(data->fd[i]);
		i++;
	}
}

void	setup_input(t_data_bonus *data, int index)
{
	if (index == 0)
	{
		if (dup2(data->fd1, STDIN_FILENO) == -1)
			exit(1);
		close(data->fd1);
	}
	else
	{
		if (dup2(data->fd[(index - 1) * 2], STDIN_FILENO) == -1)
			exit(1);
		close(data->fd[(index - 1) * 2]);
	}
}

void	setup_output(t_data_bonus *data, int index)
{
	if (index == data->cmd_num - 1)
	{
		if (dup2(data->fd2, STDOUT_FILENO) == -1)
			exit(1);
		close(data->fd2);
	}
	else
	{
		if (dup2(data->fd[index * 2 + 1], STDOUT_FILENO) == -1)
			exit(1);
		close(data->fd[index * 2 + 1]);
	}
}

void	execute_command(t_data_bonus *data, int index)
{
	char	*path;

	path = get_path(data->cmds[index][0], data->envp);
	if (!path)
		path = ft_strdup(data->cmds[index][0]);
	execve(path, data->cmds[index], data->envp);
	free(path);
}

void	son_process(t_data_bonus *data, int index, pid_t *pids)
{
	setup_input(data, index);
	setup_output(data, index);
	close_all_pipes(data);
	execute_command(data, index);
	free(data->fd);
	exit_free(data);
	free(pids);
	close(data->fd1);
	close(data->fd2);
	close(0);
	close(1);
	perror("execve");
	exit(1);
}
