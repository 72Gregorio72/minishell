/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:19:44 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/04 10:05:00 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexing	*find_next_command(t_lexing *start)
{
	t_lexing	*tmp;

	tmp = start;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "command", 8))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_lexing	*find_prev_command(t_lexing *start, t_lexing *end)
{
	t_lexing	*tmp;
	t_lexing	*command;

	tmp = start;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "command", 8))
		{
			command = tmp;
			while (tmp)
			{
				if (!ft_strncmp(tmp->type, "and_operator", 13)
					|| !ft_strncmp(tmp->type, "or_operator", 12)
					|| !ft_strncmp(tmp->type, "open_parenthesis", 17)
					|| !ft_strncmp(tmp->type, "close_parenthesis", 18)
					|| !ft_strncmp(tmp->type, "pipe", 4))
					break ;
				if (!ft_strncmp(tmp->value, end->value, ft_strlen(end->value)))
					return (command);
				tmp = tmp->next;
			}
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	find_red(t_lexing *lst, t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*command;

	tmp = lst;
	while (tmp && ft_strncmp(tmp->type, "and_operator", 13) != 0)
	{
		if (!ft_strncmp(tmp->type, "infile", 7))
		{
			command = find_next_command(tmp);
			if (!command)
				return (error_exit(gen, "minishell: syntax error", 2));
			if (!access(tmp->value, F_OK))
				command->infile = open(tmp->value, O_RDONLY);
			else
				error_exit(gen, "minishell: no such file or directory", 1);
		}
		else if (!ft_strncmp(tmp->type, "outfile", 8))
		{
			command = find_prev_command(lst, tmp);
			if (!command)
				return (error_exit(gen, "minishell: syntax error", 2));
			command->outfile = open(tmp->value, O_CREAT | O_WRONLY, 0644);
		}
		tmp = tmp->next;
	}
}
