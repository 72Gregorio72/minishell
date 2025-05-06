/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:15:26 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 09:58:24 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_prev(t_lexing *node, int *i)
{
	if (node && node->prev && node->prev->prev && !ft_strncmp(node->type, "command", 8)
		&& !ft_strncmp(node->prev->type, "infile", 7)
		&& !ft_strncmp(node->prev->prev->type, "redirect_input", 15))
	{
		(*i) += 2;
		return (1);
	}
	return (0);
}

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
		else
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

t_lexing	*clean_data(t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*head;
	t_lexing	*new_node;

	tmp = gen->lexed_data;
	head = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, "argument", 9)
			&& ft_strncmp(tmp->type, "option", 7)
			&& ft_strncmp(tmp->type, "open_parenthesis", 17)
			&& ft_strncmp(tmp->type, "close_parenthesis", 18)
			&& !check_redirect(tmp)
			&& ft_strncmp(tmp->type, "infile", 7)
			&& ft_strncmp(tmp->type, "outfile", 8)
			&& ft_strncmp(tmp->type, "here_doc_delimiter", 19))
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
	new_node = head;
	while (new_node)
	{
		if (new_node->command)
		{
			int i = 0;
			while (new_node->command[i])
				i++;
		}
		new_node = new_node->next;
	}
	return (head);
}

int	parsing(t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*tmp2;

	gen->root = NULL;
	if (!quote_handler(gen, gen->lexed_data)
		|| !check_here_doc(gen, gen->lexed_data)
		|| !find_red(gen->lexed_data, gen, 0)
		|| !check_wildcards(gen, gen->lexed_data)
		|| !check_operators(gen, gen->lexed_data))
		return (0);
	if (!loop_expand(gen))
		return (0);
	if (!ft_strncmp(gen->lexed_data->value, "poke", 4))
		ft_pokemon(gen);
	gen->cleaned_data = clean_data(gen);
	here_doccer(gen->lexed_data, gen->cleaned_data);
	tmp = gen->cleaned_data;
	tmp2 = gen->lexed_data;
	if (ft_lstsize(gen->cleaned_data) != 2)
		gen->root = fill_tree(gen->cleaned_data,
				ft_lstlast(gen->cleaned_data), gen->root);
	if (find_cmd_num(tmp2) > 1)
		exec_command(gen);
	else
		exec_single_command(gen, tmp);
	ft_treeclear(gen->root);
	return (1);
}
