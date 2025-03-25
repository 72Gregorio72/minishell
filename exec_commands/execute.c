/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/03/25 11:19:35 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	exec_command(t_gen *gen, t_lexing *node)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**env;

	env = copy_matrix(gen->my_env);
	if (!node || !node->value)
		return ;
	cmd_path = get_path(node->value, env);
	if (!cmd_path)
	{
		cmd_path = ft_strdup(node->value);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("fork error\n", 2);
		gen->exit_status = 1;
		free(cmd_path);
		return ;
	}
	if (pid == 0)
	{
		execve(cmd_path, gen->av, env);
		ft_putstr_fd("execve error\n", 2);
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			gen->exit_status = WEXITSTATUS(status);
	}
	free_matrix(env);
	free(cmd_path);
}
