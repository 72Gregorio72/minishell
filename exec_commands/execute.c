/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/21 16:21:53 by gpicchio         ###   ########.fr       */
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

int	find_cmd_num(t_lexing *node)
{
	int			cmd_num;
	t_lexing	*tmp;

	cmd_num = 0;
	tmp = node;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "command", 7) || check_redirect(tmp))
			cmd_num++;
		if (tmp->next && tmp->next->next
			&& !ft_strncmp(tmp->type, "pipe", 4)
			&& (!ft_strncmp(tmp->next->next->type, "outfile", 8)
				|| !ft_strncmp(tmp->next->next->type, "infile", 7))
			&& ((tmp->next->next->next
					&& !ft_strncmp(tmp->next->next->next->type, "pipe", 4))
				|| !tmp->next->next->next))
			cmd_num++;
		tmp = tmp->next;
	}
	return (cmd_num);
}

void	collect_piped_cmds(t_tree *node, t_lexing **cmds, int *i, t_gen *gen)
{
	if (!node)
		return ;
	collect_piped_cmds(node->left, cmds, i, gen);
	if (node->data->command
		&& (!ft_strncmp(node->data->command[0], ">", 1)
			|| !ft_strncmp(node->data->command[0], "<", 1))
		&& node->data->command[1] && !node->data->command[2])
	{
		find_red(node->data, gen);
		return ;
	}
	if (node->data && !ft_strncmp(node->data->type, "command", 7))
		cmds[(*i)++] = node->data;
	collect_piped_cmds(node->right, cmds, i, gen);
}

int	check_here_doc_command(char **command)
{
	int	i;
	int	j;

	i = 0;
	while (command[i])
	{
		if (!ft_strncmp(command[i], "<<", 2))
		{
			if (!command[i + 1])
				return (-1);

			free(command[i]);
			free(command[i + 1]);
			j = i;
			while (command[j + 2])
			{
				command[j] = command[j + 2];
				j++;
			}
			command[j] = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_other_doc(t_lexing *node)
{
	t_lexing	*tmp;

	tmp = node;
	while (tmp && !stop_check(tmp))
	{
		if (!ft_strncmp(tmp->type, "here_doc", 9))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	check_open(t_lexing *current, t_lexing **cleaned_data, t_gen *gen, int *here_doc_num)
{
	if (current->next && check_other_doc(current->next))
		handle_here_doc(current->next->value, NULL, here_doc_num, gen);
	else if (current && current->next && (*cleaned_data))
	{
		handle_here_doc(current->next->value, (*cleaned_data),
			here_doc_num, gen);
		if ((*cleaned_data) && (*cleaned_data)->next)
		{
			(*cleaned_data) = (*cleaned_data)->next;
			while ((*cleaned_data)
				&& ft_strncmp((*cleaned_data)->type, "command", 8))
			{
				(*cleaned_data) = (*cleaned_data)->next;
			}
		}
	}
	else if (!(*cleaned_data))
		handle_here_doc(current->next->value, NULL, here_doc_num, gen);
}

void	here_doccer(t_lexing *node, t_lexing *cleaned_data, t_gen *gen)
{
	t_lexing	*current;
	t_lexing	*tmp;
	int			here_doc_num;

	current = node;
	tmp = cleaned_data;
	here_doc_num = 0;
	while (current)
	{
		if (current->type && !ft_strncmp(current->type, "here_doc", 9))
			check_open(current, &tmp, gen, &here_doc_num);
		current = current->next;
	}
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
//((echo 1 && echo 2) && (echo 3 || echo 4)) || (echo 5 && echo 6)

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
