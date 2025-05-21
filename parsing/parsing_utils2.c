/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:29:37 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/21 13:14:08 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_prev_here_doc(t_lexing *tmp, int *i)
{
	while (tmp && !stop_check(tmp))
	{
		if (!ft_strncmp(tmp->type, "here_doc", 9))
			(*i) += 2;
		tmp = tmp->prev;
	}
	return (0);
}

char	**get_command(t_lexing *node)
{
	char		**command;
	t_lexing	*tmp;
	int			i;
	int			found;
	int			here_doced;

	i = 0;
	tmp = node;
	found = 0;
	if (ft_strncmp(tmp->type, "command", 8)
		&& ft_strncmp(tmp->type, "open_parenthesis", 17)
		&& !check_redirect(tmp))
		return (NULL);
	found = check_prev(tmp, &i);
	here_doced = check_prev_here_doc(tmp, &i);
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "argument", 9)
			|| !ft_strncmp(tmp->type, "option", 7)
			|| !ft_strncmp(tmp->type, "command", 8)
			|| !ft_strncmp(tmp->type, "output_append", 14)
			|| !ft_strncmp(tmp->type, "redirect_input", 15)
			|| !ft_strncmp(tmp->type, "redirect_output", 16)
			|| !ft_strncmp(tmp->type, "outfile", 8)
			|| !ft_strncmp(tmp->type, "infile", 7)
			|| (!ft_strncmp(tmp->type, "here_doc_delimiter", 19) && !here_doced)
			|| (!ft_strncmp(tmp->type, "here_doc", 9) && !here_doced))
			i++;
		tmp = tmp->next;
	}
	command = (char **)malloc(sizeof(char *) * (i + 1));
	if (!command)
		return (NULL);
	tmp = node;
	found = check_prev(tmp, &i);
	here_doced = check_prev_here_doc(tmp, &i);
	i = 0;
	if (found || here_doced)
	{
		while (tmp && tmp->prev && !stop_check(tmp))
			tmp = tmp->prev;
		while (tmp && ft_strncmp(tmp->type, "command", 8))
		{
			if (check_redirect(tmp) && tmp->next)
			{
				command[i] = ft_strdup(tmp->value);
				i++;
				command[i] = ft_strdup(tmp->next->value);
				i++;
			}
			tmp = tmp->next;
		}
	}
	tmp = node;
	while (tmp)
	{
		if (!here_doced && (!ft_strncmp(tmp->type, "here_doc_delimiter", 19)
				|| !ft_strncmp(tmp->type, "here_doc", 9)))
			here_doced = 1;
		if (!ft_strncmp(tmp->type, "argument", 9)
			|| !ft_strncmp(tmp->type, "option", 7)
			|| !ft_strncmp(tmp->type, "command", 8)
			|| !ft_strncmp(tmp->type, "output_append", 14)
			|| !ft_strncmp(tmp->type, "redirect_input", 15)
			|| !ft_strncmp(tmp->type, "redirect_output", 16)
			|| !ft_strncmp(tmp->type, "outfile", 8)
			|| !ft_strncmp(tmp->type, "infile", 7))
		{
			command[i] = ft_strdup(tmp->value);
			i++;
		}
		else if (!check_after(tmp))
			break ;
		tmp = tmp->next;
	}
	tmp = node;
	while (tmp && !stop_check(tmp))
	{
		if (!tmp->next)
			break ;
		tmp = tmp->next;
	}
	if (here_doced)
	{
		while (tmp && tmp->prev)
		{
			if (!ft_strncmp(tmp->type, "here_doc_delimiter", 19))
			{
				command[i] = ft_strdup(tmp->prev->value);
				i++;
				command[i] = ft_strdup(tmp->value);
				i++;
				break ;
			}
			tmp = tmp->prev;
		}
	}
	command[i] = NULL;
	return (command);
}

char	**ft_strdup_matrix(char **matrix)
{
	char	**new_matrix;
	int		i;

	i = 0;
	while (matrix[i])
		i++;
	new_matrix = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_matrix)
		return (NULL);
	i = 0;
	while (matrix[i])
	{
		new_matrix[i] = ft_strdup(matrix[i]);
		i++;
	}
	new_matrix[i] = NULL;
	return (new_matrix);
}

int	check_lexed(t_lexing *tmp)
{
	if (ft_strncmp(tmp->type, "argument", 9)
		&& ft_strncmp(tmp->type, "option", 7)
		&& ft_strncmp(tmp->type, "open_parenthesis", 17)
		&& ft_strncmp(tmp->type, "close_parenthesis", 18)
		&& ft_strncmp(tmp->type, "here_doc", 9)
		&& ft_strncmp(tmp->type, "infile", 7)
		&& ft_strncmp(tmp->type, "outfile", 8)
		&& ft_strncmp(tmp->type, "here_doc_delimiter", 19))
		return (1);
	return (0);
}
