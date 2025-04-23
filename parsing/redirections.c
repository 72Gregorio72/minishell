/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:19:44 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/23 13:46:39 by gpicchio         ###   ########.fr       */
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

int	check_end(t_lexing *tmp)
{
	if (!ft_strncmp(tmp->type, "and_operator", 13)
		|| !ft_strncmp(tmp->type, "or_operator", 12)
		|| !ft_strncmp(tmp->type, "open_parenthesis", 17)
		|| !ft_strncmp(tmp->type, "close_parenthesis", 18)
		|| !ft_strncmp(tmp->type, "pipe", 4))
		return (1);
	return (0);
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
				if (check_end(tmp))
					break ;
				if (!ft_strncmp(tmp->value, end->value, ft_strlen(end->value)))
					return (command);
				tmp = tmp->next;
			}
		}
		if (tmp == end)
			break ;
		if (tmp && tmp->next)
			tmp = tmp->next;
	}
	return (NULL);
}

int	find_red(t_lexing *lst, t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*redirect;

	tmp = lst;
	redirect = NULL;
	while (tmp && ft_strncmp(tmp->type, "and_operator", 13) != 0)
	{
		if (!ft_strncmp(tmp->type, "infile", 7) && !tmp->wildcard)
		{
			if (!util_infile(tmp, gen, lst))
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
