/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 09:58:04 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/23 15:51:46 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_and_export(t_gen *gen, char *succ, char *curr, t_lexing *node)
{
	if (ft_strncmp("export", curr, ft_strlen("export")) == 0
		&& ft_strlen(curr) == ft_strlen("export"))
	{
		if (!succ)
			return (ft_export(NULL, gen, node, &gen->export_env));
		return (call_export(gen, node));
	}
	else if (ft_strncmp("unset", curr, ft_strlen("unset")) == 0
		&& ft_strlen(curr) == ft_strlen("unset"))
	{
		if (!succ)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
			return (1);
		}
		else
			return (call_unset(node->command, gen));
	}
	return (0);
}

int	exec_builtin(t_gen *gen, t_lexing *node)
{
	if (ft_strncmp("echo", node->command[0], ft_strlen("echo")) == 0
		&& ft_strlen(node->command[0]) == ft_strlen("echo"))
		return (ft_echo(node, gen, node->outfile));
	else if (ft_strncmp("env", node->command[0], ft_strlen("env")) == 0
		&& ft_strlen(node->command[0]) == ft_strlen("env"))
		return (ft_env(gen->my_env, 0, node));
	else if (ft_strncmp("pwd", node->command[0], ft_strlen("pwd")) == 0
		&& ft_strlen(node->command[0]) == ft_strlen("pwd"))
		return (ft_pwd(gen->my_env, node->outfile));
	else if (ft_strncmp("exit", node->command[0], ft_strlen("exit")) == 0
		&& ft_strlen(node->command[0]) == ft_strlen("exit"))
		return (ft_exit(gen, node));
	else if (ft_strncmp("cd", node->command[0], ft_strlen("cd")) == 0
		&& ft_strlen(node->command[0]) == ft_strlen("cd"))
		return (ft_cd(node->command[1], gen->export_env, gen));
	else
		return (unset_and_export(gen, node->command[1],
				node->command[0], node));
	return (1);
}

int	check_lexed(t_lexing *tmp)
{
	if (ft_strncmp(tmp->type, "argument", 9)
		&& ft_strncmp(tmp->type, "option", 7)
		&& ft_strncmp(tmp->type, "open_parenthesis", 17)
		&& ft_strncmp(tmp->type, "close_parenthesis", 18)
		&& ft_strncmp(tmp->type, "infile", 7)
		&& ft_strncmp(tmp->type, "outfile", 8)
		&& ft_strncmp(tmp->type, "here_doc_delimiter", 19))
		return (1);
	return (0);
}

int	stop_check(t_lexing *tmp)
{
	if (!ft_strncmp(tmp->type, "pipe", 4)
		|| !ft_strncmp(tmp->type, "and_operator", 13)
		|| !ft_strncmp(tmp->type, "or_operator", 12)
		|| !ft_strncmp(tmp->type, "close_parenthesis", 18)
		|| !ft_strncmp(tmp->type, "open_parenthesis", 17))
		return (1);
	return (0);
}

void	util_filter_lexed_data(t_lexing **new_node, t_lexing **tmp)
{
	(*new_node)->outfile = (*tmp)->outfile;
	(*new_node)->infile = (*tmp)->infile;
	(*new_node)->layer = (*tmp)->layer;
	(*new_node)->expanded = (*tmp)->expanded;
}
