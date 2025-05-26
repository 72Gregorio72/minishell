/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:06:05 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/23 16:06:08 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_other_doc(t_lexing *node)
{
	t_lexing	*tmp;

	tmp = node;
	while (tmp && !stop_check(tmp))
	{
		if (!ft_strncmp(tmp->type, "here_doc", 9))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	check_open(t_lexing *current, t_lexing **cleaned_data, t_gen *gen, int *here_doc_num)
{
	if (current->next && check_other_doc(current->next))
		handle_here_doc(current->next->value, NULL, here_doc_num, gen);
	else if (current && current->next && (*cleaned_data))
	{
		handle_here_doc(current->next->value, (*cleaned_data),
			here_doc_num, gen);
		if ((*cleaned_data) && (*cleaned_data)->next)
		{
			(*cleaned_data) = (*cleaned_data)->next;
			while ((*cleaned_data)
				&& ft_strncmp((*cleaned_data)->type, "command", 8))
			{
				(*cleaned_data) = (*cleaned_data)->next;
			}
		}
	}
	else if (!(*cleaned_data))
		handle_here_doc(current->next->value, NULL, here_doc_num, gen);
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
			check_open(current, &tmp, gen, &here_doc_num);
		current = current->next;
	}
}
