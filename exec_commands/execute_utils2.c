/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:41:55 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/16 12:26:35 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* int	init_exec_data(t_gen *gen, t_lexing *node, char ***env, char **cmd_path)
{
	*env = copy_matrix(gen->my_env);
	if (!node || !node->value)
		return (0);
	*cmd_path = get_path(node->value, *env);
	if (!(*cmd_path))
		*cmd_path = ft_strdup(node->value);
	if (access(*cmd_path, F_OK | X_OK) == -1)
	{
		handle_command_not_found(gen, *cmd_path, node->value, *env);
		if (node->piped)
			cleanup_on_exit(gen);
		return (0);
	}
	return (1);
}

int	safe_fork(t_gen *gen)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("fork error\n", 2);
		gen->exit_status = 1;
		return (-1);
	}
	return (pid);
}

void	wait_and_cleanup(t_gen *gen, t_lexing *node, char *cmd_path, char **env)
{
	int	status;

	waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		gen->exit_status = WEXITSTATUS(status);
	free_matrix(env);
	if (node->piped)
		cleanup_on_exit(gen);
	free(cmd_path);
}

void	exec_single_command(t_gen *gen, t_lexing *node)
{
	int		pid;
	char	*cmd_path;
	char	**env;

	if (!prepare_command_execution(gen, node, &cmd_path, &env))
		return ;
	pid = safe_fork(gen);
	if (pid == -1)
	{
		free(cmd_path);
		return ;
	}
	if (pid == 0)
		child_process_logic(gen, node, cmd_path, env);
	wait_and_cleanup(gen, node, cmd_path, env);
}

int	find_cmd_num(t_lexing *node)
{
	int			cmd_num;
	t_lexing	*tmp;

	cmd_num = 0;
	tmp = node;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "command", 7))
			cmd_num++;
		tmp = tmp->next;
	}
	return (cmd_num);
} */
