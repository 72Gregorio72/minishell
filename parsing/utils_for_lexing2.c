/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_lexing2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:28:58 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/30 12:55:43 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_wildcards(t_gen *gen, t_lexing *lexed)
{
	t_lexing	*tmp;

	tmp = lexed;
	while (tmp)
	{
		if (tmp->wildcard
			&& ft_strncmp(tmp->type, "here_doc_delimiter", 19) != 0)
		{
			if (!ft_strncmp(tmp->type, "outfile", 8)
				|| !ft_strncmp(tmp->type, "infile", 7))
				return (error_exit(gen, "minishell: ambiguos redirect", 1), 0);
			else
				expand_wildcard(&tmp, gen);
		}
		tmp = tmp->next;
	}
	return (1);
}

static int	util_chek(t_lexing *tmp, t_lexing *succ)
{
	if (!ft_strncmp(tmp->type, "redirect_input", 15)
		&& !ft_strncmp(succ->type, "redirect_input", 15))
		return (0);
	if (!ft_strncmp(tmp->type, "redirect_output", 16)
		&& !ft_strncmp(succ->type, "redirect_output", 16))
		return (0);
	if (!ft_strncmp(tmp->type, "output_append", 14)
		&& !ft_strncmp(succ->type, "output_append", 14))
		return (0);
	if (!ft_strncmp(tmp->type, "here_doc", 9)
		&& !ft_strncmp(succ->type, "here_doc", 9))
		return (0);
	return (1);
}

int	check_files(t_gen *gen, t_lexing *lexed)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lexed;
	succ = NULL;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if (succ && !ft_strncmp(tmp->type, "redirect_input", 15)
			&& check_not_command(succ))
			return (error_exit(gen, "mininshell: syntax error", 2), 0);
		if (succ && !util_chek(tmp, succ))
			return (error_exit(gen, "mininshell: syntax error", 2), 0);
		if (succ
			&& (!ft_strncmp(tmp->type, "redirect_output", 16)
				|| !ft_strncmp(tmp->type, "output_append", 14))
			&& check_not_command(succ))
			return (error_exit(gen, "mininshell: syntax error", 2), 0);
		tmp = tmp->next;
	}
	return (1);
}
