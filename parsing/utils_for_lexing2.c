/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_lexing2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:28:58 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 11:04:27 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_wildcards(t_gen *gen)
{
	t_lexing	*tmp;

	tmp = gen->lexed_data;
	while (tmp)
	{
		if (tmp->wildcard
			&& ft_strncmp(tmp->type, "here_doc_delimiter", 19) != 0)
		{
			if (!ft_strncmp(tmp->type, "outfile", 8)
				|| !ft_strncmp(tmp->type, "infile", 7))
				return (error_exit(gen, "minishell: ambiguos redirect", 1), 0);
			else
				expand_wildcard(&tmp);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	check_files(t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = gen->lexed_data;
	succ = NULL;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if (succ && !ft_strncmp(tmp->type, "redirect_input", 15)
			&& check_not_command(succ))
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
