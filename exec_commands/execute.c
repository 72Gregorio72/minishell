/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/16 11:22:57 by vcastald         ###   ########.fr       */
=======
/*   Updated: 2025/05/14 11:56:24 by gpicchio         ###   ########.fr       */
>>>>>>> parent of 716867c... bhoooooooooooooo
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

<<<<<<< HEAD
int	prepare_command_execution(t_gen *gen, t_lexing *node,
	char **cmd_path, char ***env)
{
	int	ok;
=======
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
	else if (ft_strncmp(command, "poke", ft_strlen("poke")) == 0
		&& ft_strlen(command) == ft_strlen("poke"))
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
	collect_piped_cmds(node->right, cmds, i);
	if (node->data && node->data->piped && !ft_strncmp(node->data->type, "command", 8))
		cmds[(*i)++] = node->data;
}

void	here_doccer(t_lexing *node, t_lexing *cleaned_data, t_gen *gen)
{
	t_lexing	*current;
	t_lexing	*tmp;
	int			found;
	int			here_doc_num;

	found = 0;
	current = node;
	tmp = cleaned_data;
	here_doc_num = 0;
	while (current)
	{
		if (current->type && !ft_strncmp(current->type, "here_doc", 9))
		{
			if (current->infile == -1)
			{
				ft_putstr_fd("Error opening here_doc file\n", 2);
				return ;
			}
			if (current->next
				&& !ft_strncmp(((t_lexing *)current->next)->type,
					"here_doc_delimiter", 19)
				&& tmp)
			{
				handle_here_doc(((t_lexing *)current->next)->value,
					tmp, &here_doc_num, gen);
				if (current->next->next && ft_strncmp(current->next->next->type, "here_doc", 9))
				{
					tmp = tmp->next;
					while (tmp && ft_strncmp(tmp->type, "command", 8))
						tmp = tmp->next;
				}
			}
			else if (current->next
				&& !ft_strncmp(((t_lexing *)current->next)->type,
					"here_doc_delimiter", 19))
			{
				handle_here_doc(((t_lexing *)current->next)->value,
					NULL, &here_doc_num, gen);
			}
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
>>>>>>> parent of 716867c... bhoooooooooooooo

	if (node && !node->piped)
	{
		if (!find_red(node, gen))
			return (0);
	}
	if (node->outfile != STDOUT_FILENO)
	{
		dup2(node->outfile, STDOUT_FILENO);
		close(node->outfile);
	}
<<<<<<< HEAD
	if (handle_builtin(gen, node))
		return (0);
	ok = init_exec_data(gen, node, env, cmd_path);
	return (ok);
=======
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

/* if (root->right && (!ft_strncmp(root->right->data->type, "and_operator", 13)
|| !ft_strncmp(root->right->data->type, "or_operator", 12)))
exec_tree(gen, root->right);
if (root->left && (!ft_strncmp(root->left->data->type, "and_operator", 13)
|| !ft_strncmp(root->left->data->type, "or_operator", 12)))
exec_tree(gen, root->left);
echo 1 | echo 2 | echo 3 | ( echo 4 && echo 5)
*/

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
		// Marcare il figlio sinistro solo se è un comando diretto
		if (node->left && !ft_strncmp(node->left->data->type, "command", 7))
			node->left->data->piped = 1;

		// Marcare TUTTI i comandi nel sottoalbero destro
		mark_all_commands_piped(node->right);
	}

	flag_piped(node->left);
	flag_piped(node->right);
>>>>>>> parent of 716867c... bhoooooooooooooo
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
