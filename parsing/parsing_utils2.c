/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:29:37 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/23 15:47:48 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_prev_here_doc(t_lexing *tmp, int *i)
{
	int		found;

	found = 0;
	while (tmp && !stop_check(tmp))
	{
		if (!ft_strncmp(tmp->type, "here_doc", 9))
		{
			found = 1;
			(*i) += 2;
		}
		tmp = tmp->prev;
	}
	return (found);
}

int	count_command_num(t_lexing *tmp)
{
	int	i;

	i = 0;
	while (tmp && tmp->prev && !stop_check(tmp) && !stop_check(tmp->prev))
		tmp = tmp->prev;
	while (tmp && !stop_check(tmp))
	{
		if (ft_strncmp(tmp->type, "open_parenthesis", 17)
				&& ft_strncmp(tmp->type, "close_parenthesis", 18))
			i++;
		tmp = tmp->next;
	}
	return (i);
}

void	fill_command(t_lexing *tmp, int i, char ***command)
{
	while (tmp && tmp->prev && !stop_check(tmp) && !stop_check(tmp->prev))
		tmp = tmp->prev;
	while (tmp && !stop_check(tmp))
	{
		if (ft_strncmp(tmp->type, "open_parenthesis", 17)
				&& ft_strncmp(tmp->type, "close_parenthesis", 18)
				&& !stop_check(tmp))
		{
			(*command)[i] = ft_strdup(tmp->value);
			i++;
		}
		tmp = tmp->next;
	}
	(*command)[i] = NULL;
}

char **get_command(t_lexing *node)
{
	char		**command;
	t_lexing	*tmp;
	int			i;

	i = 0;
	tmp = node;
	i = count_command_num(tmp);
	command = (char **)malloc(sizeof(char *) * (i + 1));
	if (!command)
		return (NULL);
	i = 0;
	fill_command(tmp, i, &command);
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
