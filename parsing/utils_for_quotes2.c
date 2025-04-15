/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_quotes2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:22:26 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 10:23:05 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_checker(char *line, int i)
{
	static int	mark;

	if (line == NULL)
		return (mark);
	if (!line[i])
		return (mark = 0, 0);
	if (line[i] == '\'')
	{
		if ((i == 0 && mark == 0) || (mark == 0 && line[i - 1] != '\\'))
			mark = 1;
		else if (((i > 0 && line[i - 1] != '\\') || i == 0) && mark == 1)
			mark = 0;
	}
	if (line[i] == '\"')
	{
		if ((i == 0 && mark == 0) || (mark == 0 && line[i - 1] != '\\'))
			mark = 2;
		else if (((i > 0 && line[i - 1] != '\\') || i == 0) && mark == 2)
			mark = 0;
	}
	if (mark == 2)
		return (2);
	if (mark == 1)
		return (1);
	return (0);
}

int	unclosed_quotes(char *word)
{
	int		i;
	char	quote_char;

	i = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '\"')
		{
			quote_char = word[i];
			i++;
			while (word[i] && word[i] != quote_char)
				i++;
			if (!word[i])
			{
				if (quote_char == '\'')
					return (0);
				else
					return (2);
			}
		}
		i++;
	}
	return (1);
}
