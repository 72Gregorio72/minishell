/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:31:13 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/24 14:59:20 by vcastald         ###   ########.fr       */
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

void	command_with_quotes(t_lexing **node)
{
	int		pos;
	char	*tmp;

	pos = find_char_pos((*node)->value, "\'\"", 0);
	tmp = NULL;
	if (!pos)
	{
		pos = find_char_pos((*node)->value, "\'\"", pos + 1);
		tmp = ft_strdup((*node)->value);
		free((*node)->value);
		(*node)->value = ft_substr(tmp, 1, pos - 1);
	}
	free(tmp);
}

void	handle_quotes(t_lexing **node)
{
	int	i;
	int	bool_quote;

	i = 0;
	while ((*node)->value[i])
	{
		bool_quote = quote_checker((*node)->value, i);
		if (bool_quote != 0)
		{
			command_with_quotes(node);
			break ;
		}
		// command_with_quotes(node);
/* 		else if (quote_checker(NULL, 0) != 2
			&& quote_checker((*node)->value, i) == 1)
		{
			i++;
		} */
		i++;
	}
	quote_checker("\0", 0);
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
		else
			handle_quotes(&tmp);
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


/*
Ordine per virgolette
- controllo se ci sono (syntax error per virgolette non chiuse)
- controllo se dollaro va espanso
- tolgo virgolette
- espando dollaro
*/