/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:56:38 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/23 09:38:46 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_spaces(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

int	check_all_upper(char *word)
{
	int	i;

	i = 1;
	while (word[i])
	{
		if (ft_toupper(word[i]) != word[i])
			return (0);
		i++;
	}
	return (1);
}

int	check_not_command(t_lexing	*succ)
{
	if (!ft_strncmp(succ->type, "pipe", 4)
		|| !ft_strncmp(succ->type, "and_operator", 13)
		|| !ft_strncmp(succ->type, "or_operator", 12)
		|| !ft_strncmp(succ->type, "close_parenthesis", 18)
		|| !ft_strncmp(succ->type, "open_parenthesis", 17))
		return (1);
	return (0);
}

int	check_redirect(t_lexing *node)
{
	if (!ft_strncmp(node->type, "output_append", 14)
		|| !ft_strncmp(node->type, "here_doc", 9)
		|| !ft_strncmp(node->type, "redirect_input", 15)
		|| !ft_strncmp(node->type, "redirect_output", 16))
		return (1);
	return (0);
}
