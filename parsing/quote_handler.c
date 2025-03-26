/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:31:13 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/26 12:44:38 by vcastald         ###   ########.fr       */
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

void	clean_quotes(t_lexing **node)
{
	char	*value;
	char	*new_value;
	int		i;
	int		j;

	if (!node || !(*node) || !(*node)->value)
		return ;
	value = (*node)->value;
	new_value = (char *)malloc(strlen(value) + 1);
	if (!new_value)
		return ;
	i = 0;
	j = 0;
	while (value[i])
	{
		if (value[i] != '\'' && value[i] != '"')
			new_value[j++] = value[i];
		i++;
	}
	new_value[j] = '\0';
	free((*node)->value);
	(*node)->value = new_value;
}

void	handle_quotes(t_lexing **node, t_gen *gen)
{
	int		i;
	int		bool_quote;
	char	*tmp;
	char	*before;
	char	*after;

	i = 0;
	tmp = NULL;
	before = NULL;
	after = NULL;
	while ((*node)->value[i])
	{
		bool_quote = quote_checker((*node)->value, i);
		if (bool_quote != 0 && !(*node)->env_variable)
			clean_quotes(node);
		else if (bool_quote == 2 && (*node)->env_variable)
		{
			handle_env_variable(node, gen);
		}
		quote_checker("\0", 0);
		i++;
	}
	free(tmp);
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
			handle_quotes(&tmp, gen);
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