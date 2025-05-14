/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 14:17:37 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/14 14:23:42 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	collect_piped_cmds(t_tree *node, t_lexing **cmds, int *i)
{
	if (!node)
		return ;
	collect_piped_cmds(node->left, cmds, i);
	collect_piped_cmds(node->right, cmds, i);
	if (node->data && node->data->piped
		&& !ft_strncmp(node->data->type, "command", 8))
		cmds[(*i)++] = node->data;
}

void	update_tmp_pointer(t_lexing **tmp, t_lexing *current)
{
	if (current->next->next
		&& ft_strncmp(current->next->next->type, "here_doc", 9))
	{
		*tmp = (*tmp)->next;
		while (*tmp && ft_strncmp((*tmp)->type, "command", 8))
			*tmp = (*tmp)->next;
	}
}

void	process_here_doc_node(t_lexing *current, t_lexing **tmp,
			int *here_doc_num, t_gen *gen)
{
	int	is_valid_type;

	is_valid_type = current->next
		&& !ft_strncmp(current->next->type, "here_doc_delimiter", 19);
	if (is_valid_type && *tmp)
	{
		handle_here_doc(current->next->value, *tmp, here_doc_num, gen);
		update_tmp_pointer(tmp, current);
	}
	else if (is_valid_type)
	{
		handle_here_doc(current->next->value, NULL, here_doc_num, gen);
	}
}

void	here_doccer(t_lexing *node, t_lexing *cleaned_data, t_gen *gen)
{
	t_lexing	*current;
	t_lexing	*tmp;
	int			here_doc_num;

	current = node;
	tmp = cleaned_data;
	here_doc_num = 0;
	while (current)
	{
		if (current->type && !ft_strncmp(current->type, "here_doc", 9))
		{
			if (current->infile == -1)
				return (ft_putstr_fd("Error opening here_doc file\n", 2));
			process_here_doc_node(current, &tmp, &here_doc_num, gen);
			if (current->outfile == -1)
			{
				ft_putstr_fd("Error opening output file\n", 2);
				close(current->infile);
				return ;
			}
		}
		current = current->next;
	}
}

int	check_and_execute_subtree(t_gen *gen, t_tree *subtree)
{
	if (!subtree)
		return (1);
	if ((!ft_strncmp(subtree->data->type, "and_operator", 13)
			|| !ft_strncmp(subtree->data->type, "or_operator", 12)))
	{
		exec_tree(gen, subtree);
		if (gen->exit_status != 0)
			return (0);
	}
	return (1);
}
