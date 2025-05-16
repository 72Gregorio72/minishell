/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:29:37 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/16 14:56:07 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_command(t_lexing *node)
{
	char		**command;
	t_lexing	*tmp;
	int			i;
	int			found;

	i = 0;
	tmp = node;
	found = 0;
	if (ft_strncmp(tmp->type, "command", 8)
		&& ft_strncmp(tmp->type, "open_parenthesis", 17)
		&& !check_redirect(tmp))
		return (NULL);
	while (tmp)
	{
		found = check_prev(tmp, &i);
		if (!ft_strncmp(tmp->type, "argument", 9)
			|| !ft_strncmp(tmp->type, "option", 7)
			|| !ft_strncmp(tmp->type, "command", 8)
			|| !ft_strncmp(tmp->type, "output_append", 14)
			|| !ft_strncmp(tmp->type, "redirect_input", 15)
			|| !ft_strncmp(tmp->type, "redirect_output", 16)
			|| !ft_strncmp(tmp->type, "outfile", 8)
			|| !ft_strncmp(tmp->type, "infile", 7))
			i++;
		tmp = tmp->next;
	}
	command = (char **)malloc(sizeof(char *) * (i + 1));
	if (!command)
		return (NULL);
	tmp = node;
	found = check_prev(tmp, &i);
	i = 0;
	if (found)
	{
		command[i] = ft_strdup(tmp->prev->prev->value);
		i++;
		command[i] = ft_strdup(tmp->prev->value);
		i++;
	}
	while (tmp)
	{
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
		&& ft_strncmp(tmp->type, "infile", 7)
		&& ft_strncmp(tmp->type, "outfile", 8)
		&& ft_strncmp(tmp->type, "here_doc_delimiter", 19))
		return (1);
	return (0);
}
