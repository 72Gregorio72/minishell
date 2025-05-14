/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:29:37 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/14 12:32:49 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_command(t_lexing *node)
{
	char	**command;
	int		i;
	int		found;
	int		count;

	i = 0;
	found = 0;
	count = count_command_elements(node, &found);
	if (ft_strncmp(node->type, "command", 8)
		&& ft_strncmp(node->type, "open_parenthesis", 17)
		&& !check_redirect(node))
		return (NULL);
	command = (char **)malloc(sizeof(char *) * (count + 1));
	if (!command)
		return (NULL);
	fill_command_prefix(command, node, &i);
	fill_command_args(command, node, &i);
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
		&& !check_redirect(tmp)
		&& ft_strncmp(tmp->type, "infile", 7)
		&& ft_strncmp(tmp->type, "outfile", 8)
		&& ft_strncmp(tmp->type, "here_doc_delimiter", 19))
		return (1);
	return (0);
}

t_lexing	*filter_lexed_data(t_lexing *lexed_data)
{
	t_lexing	*tmp;
	t_lexing	*head;
	t_lexing	*new_node;

	tmp = lexed_data;
	head = NULL;
	while (tmp)
	{
		if (check_lexed(tmp))
		{
			new_node = ft_lstnew_cleaned(ft_strdup(tmp->value),
					ft_strdup(tmp->type), tmp->strength, get_command(tmp));
			new_node->outfile = tmp->outfile;
			new_node->infile = tmp->infile;
			new_node->layer = tmp->layer;
			new_node->expanded = tmp->expanded;
			if (!new_node)
				return (NULL);
			ft_lstadd_back(&head, new_node);
		}
		tmp = tmp->next;
	}
	return (head);
}

void	process_command_nodes(t_lexing *head)
{
	t_lexing	*new_node;
	int			i;

	new_node = head;
	while (new_node)
	{
		if (new_node->command)
		{
			i = 0;
			while (new_node->command[i])
				i++;
		}
		new_node = new_node->next;
	}
}
