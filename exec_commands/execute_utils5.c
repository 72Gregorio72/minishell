/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:01:45 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/27 11:43:16 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	util_exit_exec(t_gen *gen)
{
	int		i;
	char	*filename;
	char	*tmp;

	i = 0;
	while (i < gen->here_doc_num)
	{
		tmp = ft_itoa(i);
		filename = ft_strjoin(".here_doc_tmp", tmp);
		unlink(filename);
		free(filename);
		free(tmp);
		i++;
	}
	ft_treeclear(gen->root);
	free_matrix(gen->my_env);
	free_matrix(gen->export_env);
	ft_lstclear(gen->lexed_data, 0);
	ft_lstclear(gen->cleaned_data, 1);
	free_matrix(gen->av);
	close(gen->fd_stdin);
}

void	print_cmd_not_found(t_lexing *node, t_gen *gen)
{
	ft_putstr_fd(RED"Command ", 2);
	ft_putstr_fd(YELLOW"\"", 2);
	ft_putstr_fd(node->value, 2);
	ft_putstr_fd("\"", 2);
	ft_putstr_fd(RED" not found\n"RESET, 2);
	gen->exit_status = 127;
}

static void	util_son_piped(t_piped *piped)
{
	close(piped->pipe_fd[0]);
	dup2(piped->pipe_fd[1], STDOUT_FILENO);
	close(piped->pipe_fd[1]);
}

static void	util_close_son_hanma(t_piped *piped)
{
	if (piped->pipe_fd[0] != -1)
		close(piped->pipe_fd[0]);
	if (piped->pipe_fd[1] != -1)
		close(piped->pipe_fd[1]);
}

void	son_piped(t_gen *gen, t_piped *piped)
{
	int	flag;

	flag = 0;
	if (find_red(piped->cmds[piped->i], gen) != 0)
	{
		if (piped->i > 0)
		{
			dup2(piped->prev_pipe, STDIN_FILENO);
			close(piped->prev_pipe);
		}
		else if (piped->cmds[piped->i]->infile != STDIN_FILENO)
		{
			dup2(piped->cmds[piped->i]->infile, STDIN_FILENO);
			close(piped->cmds[piped->i]->infile);
		}
		if (piped->i < piped->num_cmds - 1)
			util_son_piped(piped);
		exec_single_command(gen, piped->cmds[piped->i]);
		flag = 1;
	}
	else
		util_close_son_hanma(piped);
	if (!flag)
		util_exit_exec(gen);
	exit(gen->exit_status);
}
