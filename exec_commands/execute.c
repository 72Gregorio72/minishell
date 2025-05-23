/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/23 16:23:38 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_single_command(t_gen *gen, t_lexing *node)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**env;

	if (node && !node->piped)
	{
		if (!find_red(node, gen))
			return ;
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
		{
			ft_treeclear(gen->root);
			free_matrix(gen->my_env);
			free_matrix(gen->export_env);
			ft_lstclear(gen->lexed_data, 0);
			ft_lstclear(gen->cleaned_data, 1);
			free_matrix(gen->av);
		}
		return ;
	}
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
		ft_putstr_fd(RED"Command ", 2);
		ft_putstr_fd(YELLOW"\"", 2);
		ft_putstr_fd(node->value, 2);
		ft_putstr_fd("\"", 2);
		ft_putstr_fd(RED" not found\n"RESET, 2);
		gen->exit_status = 127;
		free(cmd_path);
		free_matrix(env);
		if (node->piped)
		{
			ft_treeclear(gen->root);
			free_matrix(gen->my_env);
			free_matrix(gen->export_env);
			ft_lstclear(gen->lexed_data, 0);
			ft_lstclear(gen->cleaned_data, 1);
			free_matrix(gen->av);
		}
		return ;
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
		ft_putstr_fd(RED"Command ", 2);
		ft_putstr_fd(YELLOW"\"", 2);
		ft_putstr_fd(node->value, 2);
		ft_putstr_fd("\"", 2);
		ft_putstr_fd(RED" not found\n"RESET, 2);
		gen->exit_status = 127;
		free_matrix(env);
		ft_treeclear(gen->root);
		free_matrix(gen->my_env);
		free_matrix(gen->export_env);
		ft_lstclear(gen->lexed_data, 0);
		ft_lstclear(gen->cleaned_data, 1);
		free_matrix(gen->av);
		free(cmd_path);
		exit(gen->exit_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			gen->exit_status = WEXITSTATUS(status);
	}
	free_matrix(env);
	if (node->piped)
	{
		ft_treeclear(gen->root);
		free_matrix(gen->my_env);
		free_matrix(gen->export_env);
		ft_lstclear(gen->lexed_data, 0);
		ft_lstclear(gen->cleaned_data, 1);
		free_matrix(gen->av);
	}
	free(cmd_path);
}

void	exec_piped_commands(t_gen *gen, t_tree *subroot)
{
	t_lexing	*cmds[256];
	int			num_cmds = 0;
	int			i, pipe_fd[2];
	int			prev_pipe = -1;
	pid_t		pid;
	int 		flag;
	t_lexing	*last_cmd;
	pid_t		last_pid;

	collect_piped_cmds(subroot, cmds, &num_cmds, gen);
	last_cmd = cmds[num_cmds - 1];
	for (i = 0; i < num_cmds; i++)
	{
		if (i < num_cmds - 1 && pipe(pipe_fd) == -1)
		{
			ft_putstr_fd("pipe error\n", 2);
			gen->exit_status = 1;
			return ;
		}
		pid = fork();
		if (!ft_strncmp(cmds[i]->value, last_cmd->value, ft_strlen(last_cmd->value)))
			last_pid = pid;
		if (pid == -1)
		{
			ft_putstr_fd("fork error\n", 2);
			gen->exit_status = 1;
			return ;
		}
		if (pid == 0)
		{
			flag = 0;
			if (find_red(cmds[i], gen) != 0)
			{
				if (i > 0)
				{
					dup2(prev_pipe, STDIN_FILENO);
					close(prev_pipe);
				}
				else if (cmds[i]->infile != STDIN_FILENO)
				{
					dup2(cmds[i]->infile, STDIN_FILENO);
					close(cmds[i]->infile);
				}
				if (i < num_cmds - 1)
				{
					close(pipe_fd[0]);
					dup2(pipe_fd[1], STDOUT_FILENO);
					close(pipe_fd[1]);
				}
				exec_single_command(gen, cmds[i]);
				flag = 1;
			}
			if (!flag)
			{
				ft_treeclear(gen->root);
				free_matrix(gen->my_env);
				free_matrix(gen->export_env);
				ft_lstclear(gen->lexed_data, 0);
				ft_lstclear(gen->cleaned_data, 1);
				free_matrix(gen->av);
			}
			exit(gen->exit_status);
		}
		if (i > 0)
			close(prev_pipe);
		if (i < num_cmds - 1)
		{
			close(pipe_fd[1]);
			prev_pipe = pipe_fd[0];
		}
	}
	for (i = 0; i < num_cmds; i++)
	{
		int status;
		pid_t pid = wait(&status);
		if (pid == last_pid && WIFEXITED(status))
			gen->exit_status = WEXITSTATUS(status);
	}
}
