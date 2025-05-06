/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:41:35 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/06 09:30:35 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lexing	*check_continue(t_lexing *lexed)
{
	t_lexing	*tmp;

	tmp = lexed;
	while (tmp)
	{
		if (tmp->env_variable && !ft_strncmp(tmp->type, "command", 8)
			&& tmp->expanded)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

t_lexing	*find_prev_node(t_lexing *end, t_lexing *start)
{
	t_lexing	*tmp;

	tmp = start;
	while (tmp)
	{
		if (tmp->next == end)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

int	loop_expand(t_gen *gen)
{
	t_lexing	*to_expand;
	t_lexing	*tmp_list;
	t_lexing	*prev;
	t_lexing	*last;
	char		**tmp_mat;

	to_expand = check_continue(gen->lexed_data);
	while (to_expand != NULL)
	{
		tmp_mat = ft_split_quote(to_expand->value, ' ');
		if (!tmp_mat)
			return (safe_free(gen), perror("malloc"),
				exit(gen->exit_status), 0);
		tmp_list = lexer(tmp_mat, gen);
		if (!tmp_list)
			return (free_matrix(tmp_mat), 0);
		if (check_files(gen, tmp_list)
			&& find_files(tmp_list, gen))
		{
			find_args(tmp_list);
			if (!layerize(gen, tmp_list))
				return (free_matrix(tmp_mat), ft_lstclear(tmp_list, 0), 0);
		}
		if (!quote_handler(gen, tmp_list)
			|| !find_red(tmp_list, gen)
			|| !check_here_doc(gen, tmp_list)
			|| !check_wildcards(gen, tmp_list)
			|| !check_operators(gen, tmp_list))
			return (free_matrix(tmp_mat), ft_lstclear(tmp_list, 0), 0);
		prev = find_prev_node(to_expand, gen->lexed_data);
		last = ft_lstlast(tmp_list);
		last->next = to_expand->next;
		if (prev)
			prev->next = tmp_list;
		else
			gen->lexed_data = tmp_list;
		ft_lstdelone(to_expand);
		free_matrix(tmp_mat);
		to_expand = check_continue(gen->lexed_data);
	}
	return (1);
}
