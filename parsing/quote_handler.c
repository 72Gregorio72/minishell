/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:31:13 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/24 12:23:10 by vcastald         ###   ########.fr       */
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


int	quote_handler(t_gen *gen)
{
	t_lexing	*tmp;

	tmp = gen->lexed_data;
	while (tmp)
	{
		if (!unclosed_quotes(tmp->value))
			return (error_exit(gen, "minishell: syntax error near \'", 2), 0);
		else if (unclosed_quotes(tmp->value) == 2)
			return (error_exit(gen, "minishell: syntax error near \"", 2), 0);
		tmp = tmp->next;
	}
	return (1);
}

/*
QUOTE CHECKER
- NULL: ultimo valore trovato
- 1: single quote prima
- 2: doubl quote prima
- passando \0: resetta statica */