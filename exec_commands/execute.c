/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/04/15 16:22:42 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *command)
{
	if (ft_strncmp(command, "cd", ft_strlen("cd")) == 0
		&& ft_strlen(command) == ft_strlen("cd"))
		return (1);
	else if (ft_strncmp(command, "echo", ft_strlen("echo")) == 0
		&& ft_strlen(command) == ft_strlen("echo"))
		return (1);
	else if (ft_strncmp(command, "env", ft_strlen("env")) == 0
		&& ft_strlen(command) == ft_strlen("env"))
		return (1);
	else if (ft_strncmp(command, "exit", ft_strlen("exit")) == 0
		&& ft_strlen(command) == ft_strlen("exit"))
		return (1);
	else if (ft_strncmp(command, "export", ft_strlen("export")) == 0
		&& ft_strlen(command) == ft_strlen("export"))
		return (1);
	else if (ft_strncmp(command, "unset", ft_strlen("unset")) == 0
		&& ft_strlen(command) == ft_strlen("unset"))
		return (1);
	else if (ft_strncmp(command, "pwd", ft_strlen("pwd")) == 0
		&& ft_strlen(command) == ft_strlen("pwd"))
		return (1);
	return (0);
}

void	here_doccer(t_lexing *node)
{
	t_lexing	*current;
	int 		found;

	found = 0;
	current = node;
	while (current)
	{
		if (current->type && !ft_strncmp(current->type, "here_doc", 9))
		{
			if (current->infile == -1)
			{
				ft_putstr_fd("Error opening here_doc file\n", 2);
				return ;
			}
			if (current->next && !ft_strncmp(((t_lexing *)current->next)->type, "here_doc_delimiter", 19))
				handle_here_doc(((t_lexing *)current->next)->value, current);
			if (current->outfile == -1)
			{
				ft_putstr_fd("Error opening output file\n", 2);
				close(current->infile);
				return ;
			}
			found = 1;
		}
		current = current->next;
	}
}

void	exec_single_command(t_gen *gen, t_lexing *node)
{
	pid_t	pid;
	int		status;
	char	*cmd_path;
	char	**env;

	here_doccer(node);
	node->outfile = 1;
	if (node->command && is_builtin(node->command[0]))
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
	cmd_path = get_path(node->command[0], env);
	if (!cmd_path)
	{
		cmd_path = ft_strdup(node->value);
	}
	if (access(cmd_path, F_OK) == -1)
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
		if (node->infile != STDIN_FILENO && node->infile > 0)
		{
			dup2(node->infile, STDIN_FILENO);
			close(node->infile);
		}
		if (node->outfile != STDOUT_FILENO && node->outfile > 0)
		{
			dup2(node->outfile, STDOUT_FILENO);
			close(node->outfile);
		}
		execve(cmd_path, node->command, env);
		ft_putstr_fd("execve error\n", 2);
		free_matrix(env);
		exit(1);
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

int		find_cmd_num(t_lexing *node)
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
}

void	collect_piped_cmds(t_tree *node, t_lexing **cmds, int *i)
{
	if (!node)
		return ;
	collect_piped_cmds(node->left, cmds, i);
	if (node->data && ft_strncmp(node->data->type, "command", 7) == 0)
		cmds[(*i)++] = node->data;
	collect_piped_cmds(node->right, cmds, i);
}

void	exec_piped_commands(t_gen *gen, t_tree *subroot)
{
	t_lexing	*cmds[256];
	int			num_cmds = 0;
	int			i, pipe_fd[2], prev_fd = -1;
	pid_t		pid;

	collect_piped_cmds(subroot, cmds, &num_cmds);
	for (i = 0; i < num_cmds; i++)
	{
		if (i < num_cmds - 1 && pipe(pipe_fd) == -1)
		{
			ft_putstr_fd("pipe error\n", 2);
			gen->exit_status = 1;
			return ;
		}
		pid = fork();
		if (pid == -1)
		{
			ft_putstr_fd("fork error\n", 2);
			gen->exit_status = 1;
			return ;
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, gen->cleaned_data->infile);
				dup2(pipe_fd[0], gen->cleaned_data->outfile);
				close(prev_fd);
			}
			if (i < num_cmds - 1)
			{
				close(pipe_fd[0]);
				dup2(pipe_fd[1], gen->cleaned_data->infile);
				close(pipe_fd[1]);
			}
			exec_single_command(gen, cmds[i]);
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
	{
		int status;
		wait(&status);
		if (i == num_cmds - 1 && WIFEXITED(status))
			gen->exit_status = WEXITSTATUS(status);
	}
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
	gen->root = NULL;
}
