/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/03/25 14:24:55 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_args(t_lexing *node)
{
	int 		i = 0;
	int 		arg_count = 0;
	t_lexing 	*tmp = node;

	while (tmp)
	{
		if (ft_strncmp(tmp->type, "argument", 8))
			arg_count++;
		tmp = tmp->next;
	}
	printf("arg_count: %d\n", arg_count);
	char **args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
	{
		ft_putstr_fd("malloc error\n", 2);
		exit(1);
	}
	tmp = node;
	while (tmp && !ft_strncmp(tmp->type, "argument", 8))
	{
		args[i++] = ft_strdup(tmp->value);
		tmp = tmp->next;
	}
	args[i] = NULL;
	/* 
	t_lexing *tmp = node;
	while (tmp)
	{
		printf("Node type: %s, Node value: %s\n", tmp->type, tmp->value);
		tmp = tmp->next;
	} */
	return (args);
}

void	exec_single_command(t_gen *gen, t_lexing *node)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**env;
	char	**args;

	args = get_args(node);
	printf("executing %s\n", node->value);
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
		execve(cmd_path, args, env);
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
void	open_pipes(int *fds, int cmd_num)
{
	int	i;

	i = 0;
	while (i < cmd_num - 1)
	{
		if (pipe(fds + i * 2) == -1)
		{
			ft_putstr_fd("pipe error\n", 2);
			exit(1);
		}
		i++;
	}
}

void	minishell_forks(t_gen *gen, t_tree *node, int *fds)
{
	pid_t	pid;
	int		status;

	if (!node)
		return;

	if (ft_strncmp(node->data->type, "command", 8) == 0)
	{
		pid = fork();
		if (pid == -1)
		{
			ft_putstr_fd("fork error\n", 2);
			exit(1);
		}
		if (pid == 0)
		{
			exec_single_command(gen, node->data);
			exit(gen->exit_status);
		}
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				gen->exit_status = WEXITSTATUS(status);
		}
	}
	else
	{
		minishell_forks(gen, node->left, fds);
		minishell_forks(gen, node->right, fds);
	}
}

void	execute_tree(t_gen *gen, t_tree *node)
{
	int		cmd_num;
	int		*fds;

	cmd_num = find_cmd_num(gen->lexed_data);
	fds = malloc(sizeof(int) * (cmd_num - 1) * 2);
	if (!fds)
	{
		ft_putstr_fd("malloc error\n", 2);
		gen->exit_status = 1;
		return ;
	}
	open_pipes(fds, cmd_num);
	minishell_forks(gen, node, fds);
	free(fds);
}

void	exec_multi_command(t_gen *gen)
{
	t_tree	*current;
	int		cmd_num;

	cmd_num = find_cmd_num(gen->lexed_data);
	current = gen->root;
	while (current->left)
		current = current->left;
	exec_single_command(gen, current->data);
}

void	exec_command(t_gen *gen)
{
	/* t_lexing	*tmp;
	int			flag;

	tmp = gen->lexed_data;
	flag = 0;
	while (tmp && !flag)
	{
		if (!ft_strncmp(tmp->type, "pipe", 4)
			|| !ft_strncmp(tmp->type, "and_operator", 13)
			|| !ft_strncmp(tmp->type, "or_operator", 12))
			flag = 1;
		tmp = tmp->next;
	}
	if (!flag)
		exec_single_command(gen, node);
	else */
		exec_multi_command(gen);
}
