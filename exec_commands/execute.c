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
	
}

void	exec_tree(t_gen *gen, t_tree *root)
{
	t_tree	*tmp;

	tmp = root;
	if (!tmp)
		return ;
	if (tmp->left)
		exec_tree(gen, tmp->left);
	if (tmp->right)
		exec_tree(gen, tmp->right);
	if (tmp->data->piped && !ft_strncmp(tmp->data->type, "command", 4))
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
	gen->root = NULL;
}
