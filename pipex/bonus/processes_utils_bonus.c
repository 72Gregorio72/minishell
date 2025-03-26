/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 10:01:12 by gpicchio          #+#    #+#             */
/*   Updated: 2025/03/25 11:56:40 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	create_pipes(t_data_bonus *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_num - 1)
	{
		if (pipe(data->fd + i * 2) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
}

void	wait_for_children(t_data_bonus *data, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < data->cmd_num)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

void	fork_processes(t_data_bonus *data, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < data->cmd_num)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			exit(1);
		}
		if (pids[i] == 0)
		{
			son_process(data, i, pids);
			exit(1);
		}
		i++;
	}
}

void	do_forks(t_data_bonus *data)
{
	pid_t	*pids;

	pids = malloc(data->cmd_num * sizeof(pid_t));
	if (!pids)
	{
		perror("malloc");
		exit(1);
	}
	fork_processes(data, pids);
	close_all_pipes(data);
	wait_for_children(data, pids);
	free(pids);
}

void	pipex(t_data_bonus *data)
{
	data->fd = malloc(((data->cmd_num - 1) * 2) * sizeof(int));
	if (!data->fd)
	{
		perror("malloc");
		exit(1);
	}
	create_pipes(data);
	do_forks(data);
	free(data->fd);
}
