/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/14 12:42:10 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	collect_piped_cmds(t_tree *node, t_lexing **cmds, int *i)
{
	if (!node)
		return ;
	collect_piped_cmds(node->left, cmds, i);
	collect_piped_cmds(node->right, cmds, i);
	if (node->data && node->data->piped
		&& !ft_strncmp(node->data->type, "command", 8))
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

	collect_piped_cmds(subroot, cmds, &num_cmds);
	if (subroot->left && !ft_strncmp(subroot->left->data->type, "and_operator", 13)
		&& !ft_strncmp(subroot->left->data->type, "or_operator", 12))
	{
		exec_tree(gen, subroot->left);
		if (gen->exit_status != 0)
			return ;
	}
	if (subroot->right && !ft_strncmp(subroot->right->data->type, "and_operator", 13)
		&& !ft_strncmp(subroot->right->data->type, "or_operator", 12))
	{
		exec_tree(gen, subroot->right);
		if (gen->exit_status != 0)
			return ;
	}
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
