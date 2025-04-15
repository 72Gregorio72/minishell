/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 09:58:04 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 12:02:55 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_and_export(t_gen *gen, char *succ, char *curr, t_lexing *node)
{
	if (ft_strncmp("export", curr, ft_strlen("export")) == 0
		&& ft_strlen(curr) == ft_strlen("export"))
	{
		if (!succ)
			return (ft_export(&gen->my_env, NULL, &gen->export_env, gen));
		return (call_export(gen, node->command));
	}
	else if (ft_strncmp("unset", curr, ft_strlen("unset")) == 0
		&& ft_strlen(curr) == ft_strlen("unset"))
	{
		if (!succ)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
			call_unset(node->command, gen);
		return (1);
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
		return (ft_env(gen->my_env, 0));
	else if (ft_strncmp("pwd", node->command[0], ft_strlen("pwd")) == 0
		&& ft_strlen(node->command[0]) == ft_strlen("pwd"))
		return (ft_pwd(gen->my_env, node->outfile));
	else if (ft_strncmp("exit", node->command[0], ft_strlen("exit")) == 0
		&& ft_strlen(node->command[0]) == ft_strlen("exit"))
		ft_exit(gen);
	else if (ft_strncmp("cd", node->command[0], ft_strlen("cd")) == 0
		&& ft_strlen(node->command[0]) == ft_strlen("cd")
		&& node->command[1])
		return (ft_cd(node->command[1], gen->export_env, gen));
	else
		return (unset_and_export(gen, node->command[1],
				node->command[0], node));
	return (1);
}
