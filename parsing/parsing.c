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

t_lexing	*clean_data(t_gen *gen)
{
	t_lexing	*head;

	head = filter_lexed_data(gen->lexed_data);
	if (!head)
		return (NULL);
	process_command_nodes(head);
	return (head);
}

int	parsing(t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*tmp2;

	gen->root = NULL;
	if (!quote_handler(gen, gen->lexed_data)
		|| !check_here_doc(gen, gen->lexed_data)
		|| !check_wildcards(gen, gen->lexed_data)
		|| !check_operators(gen, gen->lexed_data))
		return (0);
	if (!loop_expand(gen) || !loop_expand_wilds(gen))
		return (0);
	gen->cleaned_data = clean_data(gen);
	here_doccer(gen->lexed_data, gen->cleaned_data, gen);
	tmp = gen->cleaned_data;
	tmp2 = gen->lexed_data;
	if (ft_lstsize(gen->cleaned_data) != 2)
		gen->root = fill_tree(gen->cleaned_data,
				ft_lstlast(gen->cleaned_data), gen->root);
	if (find_cmd_num(tmp2) > 1)
		exec_command(gen);
	else if (gen->cleaned_data)
		exec_single_command(gen, tmp);
	return (ft_treeclear(gen->root), 1);
}
