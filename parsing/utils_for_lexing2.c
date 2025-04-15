/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_lexing2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:28:58 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/11 12:30:17 by vcastald         ###   ########.fr       */
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
