/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:59:52 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/23 16:23:02 by vcastald         ###   ########.fr       */
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

int check_no_comm_after(t_lexing *start)
{
	t_lexing	*tmp;

	tmp = start;
	if (stop_check(tmp))
		tmp = tmp->next;
	while (tmp && !stop_check(tmp))
	{
		if (!ft_strncmp(tmp->type, "command", 8))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

int	find_cmd_num(t_lexing *node)
{
	int			cmd_num;
	t_lexing	*tp;

	cmd_num = 0;
	tp = node;
	while (tp)
	{
		if ((stop_check(tp) || check_redirect(tp)) && check_no_comm_after(tp))
		{
			cmd_num++;
			tp = tp->next;
			while (tp && !stop_check(tp))
				tp = tp->next;
		}
		if (tp && (!ft_strncmp(tp->type, "command", 8)))
			cmd_num++;
		if (tp)
			tp = tp->next;
	}
	return (cmd_num);
}

void	collect_piped_cmds(t_tree *node, t_lexing **cmds, int *i, t_gen *gen)
{
	if (!node)
		return ;
	collect_piped_cmds(node->left, cmds, i, gen);
	if (node->data && (!ft_strncmp(node->data->type, "command", 7)
			|| check_redirect(node->data)))
		cmds[(*i)++] = node->data;
	if (node->data->command && node->data->command[0]
		&& (!ft_strncmp(node->data->command[0], ">", 1)
			|| !ft_strncmp(node->data->command[0], "<", 1))
		&& node->data->command[1] && !node->data->command[2])
	{
		find_red(node->data, gen);
		return ;
	}
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
