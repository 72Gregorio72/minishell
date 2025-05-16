/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:39:05 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/16 12:26:44 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* int	is_builtin(char *command)
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

void	cleanup_on_exit(t_gen *gen)
{
	ft_treeclear(gen->root);
	free_matrix(gen->my_env);
	free_matrix(gen->export_env);
	ft_lstclear(gen->lexed_data, 0);
	ft_lstclear(gen->cleaned_data, 1);
	free_matrix(gen->av);
	close(gen->fd_stdin);
}

int	handle_builtin(t_gen *gen, t_lexing *node)
{
	if (node && node->command && node->command[0]
		&& is_builtin(node->command[0]))
	{
		if (exec_builtin(gen, node))
			gen->exit_status = 0;
		else if (gen->exit_status != 1)
			gen->exit_status = 127;
		if (node->piped)
			cleanup_on_exit(gen);
		return (1);
	}
	return (0);
}

void	handle_command_not_found(t_gen *gen, char *cmd_path,
	char *value, char **env)
{
	ft_putstr_fd(RED"Command ", 2);
	ft_putstr_fd(YELLOW"\"", 2);
	ft_putstr_fd(value, 2);
	ft_putstr_fd("\"", 2);
	ft_putstr_fd(RED" not found\n"RESET, 2);
	gen->exit_status = 127;
	free(cmd_path);
	free_matrix(env);
	close(gen->fd_stdin);
}

void	child_process_logic(t_gen *gen, t_lexing *node,
	char *cmd_path, char **env)
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
	cleanup_on_exit(gen);
	free(cmd_path);
	close(gen->fd_stdin);
	exit(gen->exit_status);
}
 */