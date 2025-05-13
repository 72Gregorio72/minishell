/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_layer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:44:49 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/13 09:33:39 by vcastald         ###   ########.fr       */
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
	while (tmp && ft_strncmp(tmp->type, "close_parenthesis", 18))
	{
		if (tmp->next && (!ft_strncmp(tmp->type, "redirect_output", 16)
				|| !ft_strncmp(tmp->type, "output_append", 14))
			&& !ft_strncmp(tmp->next->type, "outfile", 8))
			return (0);
		else if (tmp->next && !ft_strncmp(tmp->type, "here_doc", 9)
			&& !ft_strncmp(tmp->next->type, "here_doc_delimiter", 19))
			return (0);
		else if (tmp->next && !ft_strncmp(tmp->type, "redirect_input", 15)
			&& !ft_strncmp(tmp->next->type, "infile", 7))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
