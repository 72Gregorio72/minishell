/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:27:36 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/21 12:03:04 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_prev(t_lexing *node, int *i)
{
	t_lexing	*tmp;
	int			found;

	if (!node->prev)
		return (0);
	tmp = node->prev;
	found = 0;
	while (tmp && !stop_check(tmp))
	{
		if (!ft_strncmp(tmp->type, "infile", 7))
		{
			(*i) += 2;
			found = 1;
		}
		if (!ft_strncmp(tmp->type, "outfile", 8))
		{
			(*i) += 2;
			found = 1;
		}
		tmp = tmp->prev;
	}
	return (found);
}

int	is_valid_command_type(const char *type)
{
	return (!ft_strncmp(type, "argument", 9)
		|| !ft_strncmp(type, "option", 7)
		|| !ft_strncmp(type, "command", 8)
		|| !ft_strncmp(type, "output_append", 14)
		|| !ft_strncmp(type, "redirect_input", 15)
		|| !ft_strncmp(type, "redirect_output", 16)
		|| !ft_strncmp(type, "outfile", 8)
		|| !ft_strncmp(type, "infile", 7));
}

int	count_command_elements(t_lexing *node, int *found)
{
	int			count;
	t_lexing	*tmp;

	count = 0;
	tmp = node;
	while (tmp)
	{
		*found = check_prev(tmp, &count);
		if (is_valid_command_type(tmp->type))
			count++;
		tmp = tmp->next;
	}
	return (count);
}

void	fill_command_prefix(char **command, t_lexing *node, int *i)
{
	int	found;

	found = check_prev(node, i);
	if (found)
	{
		command[(*i)++] = ft_strdup(node->prev->prev->value);
		command[(*i)++] = ft_strdup(node->prev->value);
	}
}

void	fill_command_args(char **command, t_lexing *tmp, int *i)
{
	while (tmp)
	{
		if (is_valid_command_type(tmp->type))
		{
			command[(*i)++] = ft_strdup(tmp->value);
		}
		else if (!check_after(tmp))
			break ;
		tmp = tmp->next;
	}
}
