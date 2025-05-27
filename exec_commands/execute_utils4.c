/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:59:16 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/27 10:20:31 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process_single_command(t_lexing *node, char **env,
			char *cmd_path, t_gen *gen)
{
	if (node->infile != STDIN_FILENO)
	{
		dup2(node->infile, STDIN_FILENO);
		if (node->infile != -1)
			close(node->infile);
	}
	if (node->outfile != STDOUT_FILENO)
	{
		dup2(node->outfile, STDOUT_FILENO);
		if (node->outfile != -1)
			close(node->outfile);
	}
	execve(cmd_path, node->command, env);
	print_cmd_not_found(node, gen);
	free_matrix(env);
	util_exit_exec(gen);
	free(cmd_path);
	exit(gen->exit_status);
}

static int	reds_builtin_single_command(t_lexing *node, t_gen *gen)
{
	if (node && !node->piped)
	{
		if (!find_red(node, gen))
			return (0);
	}
	if (node && node->command
		&& node->command[0] && is_builtin(node->command[0]))
	{
		if (exec_builtin(gen, node))
			gen->exit_status = 0;
		else
		{
			if (gen->exit_status != 1)
				gen->exit_status = 127;
		}
		if (node->piped)
			util_exit_exec(gen);
		return (0);
	}
	return (1);
}

static void	fork_single_cmd(t_gen *gen, char **env, char *cmd_path,
			t_lexing *node)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("fork error\n", 2);
		gen->exit_status = 1;
		free(cmd_path);
		return ;
	}
	if (pid == 0)
		child_process_single_command(node, env, cmd_path, gen);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			gen->exit_status = WEXITSTATUS(status);
	}
	free_matrix(env);
	if (node->piped)
		util_exit_exec(gen);
	free(cmd_path);
}

void	exec_single_command(t_gen *gen, t_lexing *node)
{
	char	*cmd_path;
	char	**env;

	if (!reds_builtin_single_command(node, gen))
		return ;
	env = copy_matrix(gen->my_env);
	if (!node || !node->value)
		return ;
	cmd_path = get_path(node->value, env);
	if (!cmd_path)
	{
		cmd_path = ft_strdup(node->value);
	}
	if (access(cmd_path, F_OK | X_OK) == -1)
	{
		print_cmd_not_found(node, gen);
		free(cmd_path);
		free_matrix(env);
		if (node->piped)
			util_exit_exec(gen);
		return ;
	}
	fork_single_cmd(gen, env, cmd_path, node);
}
