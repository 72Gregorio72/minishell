/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/04/02 16:49:37 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_single_command(t_gen *gen, t_lexing *node)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**env;

	if (exec_builtin(gen, node))
		return ;
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
		execve(cmd_path, node->command, env);
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

int		find_cmd_num(t_lexing *node)
{
	int			cmd_num;
	t_lexing	*tmp;

	cmd_num = 1;
	tmp = node;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, "command", 8))
			cmd_num++;
		tmp = tmp->next;
	}
	return (cmd_num);
}

void	exec_piped_commands(t_gen *gen)
{
	t_tree	*nodes[256];
	int		num_cmds = 0;
	int		i;
	int		pipe_fd[2];
	int		prev_fd = -1;
	pid_t	pid;

	t_tree *curr = gen->root;
	while (curr && ft_strncmp(curr->data->type, "pipe", 4) == 0)
	{
		nodes[num_cmds++] = curr->left;
		curr = curr->right;
	}
	if (curr)
		nodes[num_cmds++] = curr;

	for (i = 0; i < num_cmds; i++)
	{
		if (i < num_cmds - 1 && pipe(pipe_fd) == -1)
		{
			ft_putstr_fd("pipe error\n", 2);
			gen->exit_status = 1;
			return;
		}

		pid = fork();
		if (pid == -1)
		{
			ft_putstr_fd("fork error\n", 2);
			gen->exit_status = 1;
			return;
		}

		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			if (i < num_cmds - 1)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
			}
			exec_single_command(gen, nodes[i]->data);
			exit(gen->exit_status);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (i < num_cmds - 1)
		{
			close(pipe_fd[1]);
			prev_fd = pipe_fd[0];
		}
	}
	for (i = 0; i < num_cmds; i++)
		wait(NULL);
}


void	exec_tree(t_gen *gen, t_tree *root)
{
	t_tree	*tmp;

	tmp = root;
	if (!tmp)
		return ;
	// if (tmp->left)
	// 	exec_tree(gen, tmp->left);
	// if (tmp->right)
	// 	exec_tree(gen, tmp->right);
	if (tmp->data->piped)
	{
		exec_piped_commands(gen);
	}
	else
	{
		exec_single_command(gen, tmp->data);
	}
}

void	flag_piped(t_tree *node)
{
	t_tree	*tmp;

	if (!node)
		return ;
	tmp = node;
	if (ft_strncmp(tmp->data->type, "pipe", 4) == 0)
	{
		tmp->data->piped = 1;
		if (tmp->right)
			tmp->right->data->piped = 1;
		if (tmp->left)
			tmp->left->data->piped = 1;
	}
	flag_piped(node->left);
	flag_piped(node->right);
}

void	init_piped(t_tree *node)
{
	t_tree	*tmp;

	if (!node)
		return ;
	tmp = node;
	tmp->data->piped = 0;
	init_piped(node->left);
	init_piped(node->right);
}

void	exec_command(t_gen *gen)
{
	init_piped(gen->root);
	flag_piped(gen->root);
	exec_tree(gen, gen->root);
	ft_treeclear(gen->root);
	free_matrix(gen->export_env);
	gen->root = NULL;
}
