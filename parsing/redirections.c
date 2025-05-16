/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:19:44 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/14 15:35:51 by gpicchio         ###   ########.fr       */
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

void	remove_redirections(t_lexing *node)
{
	int		i;
	int		j;
	int		mat_length;
	char	**tmp;

	i = 0;
	j = 0;
	mat_length = calc_mat_len(node, &i);
	tmp = malloc(sizeof(char *) * (mat_length + 1));
	if (!tmp)
		return ;
	while (node->command[i])
	{
		if (ft_strncmp(node->command[i], "<", 1)
			&& ft_strncmp(node->command[i], ">>", 2)
			&& ft_strncmp(node->command[i], ">", 1))
			tmp[j++] = ft_strdup(node->command[i]);
		else
			i++;
		i++;
	}
	tmp[j] = NULL;
	free_matrix(node->command);
	node->command = copy_matrix(tmp);
	free_matrix(tmp);
}

int	find_red(t_lexing *node, t_gen *gen)
{
	int	i;
	int	val;

	i = 0;
	val = -1;
	while (node->command && node->command[i])
	{
		if (!ft_strncmp(node->command[i], "<", 1))
			val = util_infile(node->command[i + 1], gen, node);
		else if (!ft_strncmp(node->command[i], ">>", 2))
			val = util_outfile(node->command[i + 1], gen, node, 2);
		else if (!ft_strncmp(node->command[i], ">", 1))
			val = util_outfile(node->command[i + 1], gen, node, 1);
		if (val == 2)
			return (0);
		i++;
	}
	if (val != -1)
		remove_redirections(node);
	return (1);
}
