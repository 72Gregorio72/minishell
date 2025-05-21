/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:41:35 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/21 10:16:46 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexing	*check_continue(t_lexing *lexed, int flag)
{
	t_lexing	*tmp;

	tmp = lexed;
	if (flag)
	{
		while (tmp)
		{
			if (tmp->env_variable && !ft_strncmp(tmp->type, "command", 8)
				&& tmp->expanded)
				return (tmp);
			tmp = tmp->next;
		}
	}
	else
	{
		while (tmp)
		{
			if (tmp->wildcard && tmp->expanded)
				return (tmp);
			tmp = tmp->next;
		}
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

t_lexing	*prechecks(t_lexing *to_expand, t_gen *gen)
{
	t_lexing	*tmp_list;
	char		**tmp_mat;

	tmp_mat = ft_split_quote(to_expand->value, ' ');
	if (!tmp_mat)
		return (safe_free(gen), perror("malloc"),
			exit(gen->exit_status), NULL);
	tmp_list = lexer(tmp_mat, gen);
	if (!tmp_list)
		return (free_matrix(tmp_mat), NULL);
	if (check_files(gen, tmp_list)
		&& find_files(tmp_list, gen))
	{
		find_args(tmp_list);
		if (!layerize(gen, tmp_list))
			return (free_matrix(tmp_mat), ft_lstclear(tmp_list, 0), NULL);
	}
	if (!quote_handler(gen, tmp_list)
		|| !check_here_doc(gen, tmp_list)
		|| !check_wildcards(gen, tmp_list)
		|| !check_operators(gen, tmp_list))
		return (free_matrix(tmp_mat), ft_lstclear(tmp_list, 0), NULL);
	return (free_matrix(tmp_mat), tmp_list);
}

void	util_expand(t_lexing **to_free_head, t_lexing *to_expand)
{
	t_lexing	*to_free_next;

	if (!(*to_free_head))
		*to_free_head = to_expand;
	else
	{
		to_free_next = ft_lstlast(*to_free_head);
		to_free_next->next = to_expand;
		to_expand->prev = to_free_next;
	}
}

int	loop_expand(t_gen *gen)
{
	t_lexing	*to_expand;
	t_lexing	*tmp_list;
	t_lexing	*prev;
	t_lexing	*last;
	t_lexing	*to_free_head;

	to_free_head = NULL;
	to_expand = check_continue(gen->lexed_data, 1);
	while (to_expand != NULL)
	{
		tmp_list = prechecks(to_expand, gen);
		if (!tmp_list)
			return (ft_lstclear(to_free_head, 0), 0);
		prev = find_prev_node(to_expand, gen->lexed_data);
		last = ft_lstlast(tmp_list);
		last->next = to_expand->next;
		if (to_expand->next)
			to_expand->next->prev = last;
		position(prev, tmp_list, gen, to_expand);
		util_expand(&to_free_head, to_expand);
		to_expand = check_continue(gen->lexed_data, 1);
	}
	ft_lstclear(to_free_head, 0);
	return (1);
}
