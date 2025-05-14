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
	if (!loop_expand(gen))
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
