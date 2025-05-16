/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_wilds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:16:39 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/16 09:23:49 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	util_args(t_lexing *tmp)
{
	if (!ft_strncmp(tmp->type, "here_doc", 9) && tmp->next
		&& !ft_strncmp(tmp->next->type, "command", 8))
		tmp->next->type = ft_strdup("here_doc_delimiter");
	else if (!ft_strncmp(tmp->type, "command", 8))
	{
		if (tmp->value[0] == '-')
			tmp->type = ft_strdup("option");
		else
			tmp->type = ft_strdup("argument");
	}
}

static void	new_find_args(t_lexing *lst)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lst;
	succ = NULL;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if (!ft_strncmp(tmp->type, "here_doc", 9) && tmp->next
			&& !ft_strncmp(succ->type, "command", 8))
			succ->type = ft_strdup("here_doc_delimiter");
		else if (!ft_strncmp(tmp->type, "command", 8))
		{
			while (tmp && !check_not_command(tmp))
			{
				util_args(tmp);
				tmp = tmp->next;
			}
		}
		if (tmp)
			tmp = tmp->next;
	}
}

static t_lexing	*prechecks_wilds(t_lexing *to_expand, t_gen *gen)
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
		new_find_args(tmp_list);
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

int	loop_expand_wilds(t_gen *gen)
{
	t_lexing	*to_expand;
	t_lexing	*tmp_list;
	t_lexing	*prev;
	t_lexing	*last;

	to_expand = check_continue(gen->lexed_data, 0);
	while (to_expand != NULL)
	{
		tmp_list = prechecks_wilds(to_expand, gen);
		if (!tmp_list)
			return (0);
		prev = find_prev_node(to_expand, gen->lexed_data);
		last = ft_lstlast(tmp_list);
		last->next = to_expand->next;
		position(prev, tmp_list, gen);
		ft_lstdelone(to_expand);
		to_expand = check_continue(gen->lexed_data, 0);
	}
	return (1);
}

