/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:29:37 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/20 16:15:36 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* char	**get_command(t_lexing *node)
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
} */

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
		{
			(*i) += 2;
			return (1);
		}
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
	while (tmp)
	{
		found = check_prev(tmp, &i);
		here_doced = check_prev_here_doc(tmp, &i);
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
	if (found)
	{
		while (tmp && tmp->prev && tmp->prev->prev && !stop_check(tmp))
		{
			if (!ft_strncmp(tmp->prev->type, "outfile", 8)
				|| !ft_strncmp(tmp->prev->type, "infile", 7))
			{
				command[i] = ft_strdup(tmp->prev->prev->value);
				i++;
				command[i] = ft_strdup(tmp->prev->value);
				i++;
			}
			tmp = tmp->prev;
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
	while(tmp && !stop_check(tmp))
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
			// char **command = get_command(tmp);
			// if (command && command[0])
			// {
			// 	for (int i = 0; command[i]; i++)
			// 		printf("Command[%d]: %s\n", i, command[i]);
			// 	printf("--------------------------\n");
			// }
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
