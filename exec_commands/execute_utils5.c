/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:01:45 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/16 12:25:59 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* #include "minishell.h"

int	execute_piped_loop(t_gen *gen, t_lexing **cmds,
	int num_cmds, pid_t *last_pid)
{
	t_data		*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (0);
	data->i = 0;
	data->prev_pipe = -1;
	data->num_cmd = num_cmds;
	gen->last_cmd = cmds[data->num_cmd - 1];
	while (data->i < data->num_cmd)
	{
		if (setup_and_fork_child(gen, cmds, data, last_pid))
		{
			ft_putstr_fd("pipe or fork error\n", 2);
			gen->exit_status = 1;
			return (0);
		}
		(data->i)++;
	}
	free(data);
	return (1);
}

void	exec_piped_commands(t_gen *gen, t_tree *subroot)
{
	t_lexing	*cmds[256];
	int			num_cmds;
	pid_t		last_pid;

	if (!prepare_piped_execution(gen, subroot, cmds, &num_cmds))
		return ;
	if (!execute_piped_loop(gen, cmds, num_cmds, &last_pid))
		return ;
	wait_for_piped_children(gen, num_cmds, last_pid);
}

void	exec_tree(t_gen *gen, t_tree *root)
{
	if (!root)
		return ;
	if (ft_strncmp(root->data->type, "pipe", 4) == 0)
	{
		exec_piped_commands(gen, root);
	}
	else if (ft_strncmp(root->data->type, "command", 8) == 0)
	{
		exec_single_command(gen, root->data);
	}
	else if (ft_strncmp(root->data->type, "or_operator", 12) == 0)
	{
		exec_tree(gen, root->left);
		if (gen->exit_status != 0)
			exec_tree(gen, root->right);
	}
	else if (ft_strncmp(root->data->type, "and_operator", 13) == 0)
	{
		exec_tree(gen, root->left);
		if (gen->exit_status == 0)
			exec_tree(gen, root->right);
	}
}

void	mark_all_commands_piped(t_tree *node)
{
	if (!node)
		return ;
	if (!ft_strncmp(node->data->type, "command", 7))
		node->data->piped = 1;
	mark_all_commands_piped(node->left);
	mark_all_commands_piped(node->right);
}

void	flag_piped(t_tree *node)
{
	if (!node)
		return ;
	if (!ft_strncmp(node->data->type, "pipe", 4))
	{
		if (node->left && !ft_strncmp(node->left->data->type, "command", 7))
			node->left->data->piped = 1;
		mark_all_commands_piped(node->right);
	}
	flag_piped(node->left);
	flag_piped(node->right);
} */
