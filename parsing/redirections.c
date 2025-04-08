/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:19:44 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/08 09:26:39 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexing	*find_next_node(t_lexing *start, char *to_find)
{
	t_lexing	*tmp;

	tmp = start;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, to_find, ft_strlen(to_find)))
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

int	util_infile(t_lexing *tmp, t_gen *gen)
{
	t_lexing	*command;

	command = find_prev_command(gen->lexed_data, tmp);
	if (!command)
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	if (!access(tmp->value, F_OK))
		command->infile = open(tmp->value, O_RDONLY);
	else
		return (error_exit(gen, "minishell: no such file or directory", 1), 0);
	return (1);
}

int	util_outfile(t_lexing *tmp, t_gen *gen, t_lexing *redirect, t_lexing *lst)
{
	t_lexing	*command;

	command = find_prev_command(lst, tmp);
	if (!command)
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	redirect = find_next_node(command, "redirect_output");
	if (!redirect)
		command->outfile = open(tmp->value,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		command->outfile = open(tmp->value,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	return (1);
}

int	find_red(t_lexing *lst, t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*redirect;
	t_lexing	*command;

	tmp = lst;
	redirect = NULL;
	command = NULL;
	while (tmp && ft_strncmp(tmp->type, "and_operator", 13) != 0)
	{
		if (!ft_strncmp(tmp->type, "infile", 7) && !tmp->wildcard)
		{
			if (!util_infile(tmp, gen))
				return (0);
		}
		else if (!ft_strncmp(tmp->type, "outfile", 8) && !tmp->wildcard)
		{
			if (!util_outfile(tmp, gen, redirect, lst))
				return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}
