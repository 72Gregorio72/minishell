/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_layer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:44:49 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/21 09:20:28 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checks_layer(t_lexing *tmp, t_lexing *succ, t_gen *gen, t_lexing *lst)
{
	if (!ft_strncmp(tmp->type, "command", 8)
		&& !ft_strncmp(succ->type, "open_parenthesis", 17))
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	if (!ft_strncmp(tmp->type, "close_parenthesis", 18)
		&& !check_not_opened(tmp, lst))
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	if (!ft_strncmp(succ->type, "close_parenthesis", 18)
		&& !check_close(tmp, succ))
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	return (1);
}

int	check_reds_in_parenth(t_lexing *node)
{
	t_lexing	*tmp;

	tmp = node;
	tmp = tmp->next;
	if (tmp->next && (!ft_strncmp(tmp->type, "redirect_output", 16)
			|| !ft_strncmp(tmp->type, "output_append", 14))
		&& !ft_strncmp(tmp->next->type, "outfile", 8)
		&& tmp->next->next
		&& !ft_strncmp(tmp->next->next->type, "close_parenthesis", 18))
		return (0);
	else if (tmp->next && !ft_strncmp(tmp->type, "here_doc", 9)
		&& !ft_strncmp(tmp->next->type, "here_doc_delimiter", 19)
		&& tmp->next->next
		&& !ft_strncmp(tmp->next->next->type, "close_parenthesis", 18))
		return (0);
	else if (tmp->next && !ft_strncmp(tmp->type, "redirect_input", 15)
		&& !ft_strncmp(tmp->next->type, "infile", 7)
		&& tmp->next->next
		&& !ft_strncmp(tmp->next->next->type, "close_parenthesis", 18))
		return (0);
	else
		return (1);
}

int	check_pipes_in_parenth(t_lexing *node)
{
	t_lexing	*tmp;

	tmp = node;
	if (tmp->prev && !ft_strncmp(tmp->prev->type, "pipe", 4))
	{
		while (tmp && ft_strncmp(tmp->type, "close_parenthesis", 18))
		{
			if (!ft_strncmp(tmp->type, "and_operator", 13)
				|| !ft_strncmp(tmp->type, "or", 12))
				return (0);
			tmp = tmp->next;
		}
	}
	return (1);
}
