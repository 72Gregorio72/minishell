/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:15:45 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 10:16:30 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*lex_quotes(int quote_type, int *i, char *word)
{
	int		start;
	char	*final;
	char	quote_char;

	start = *i;
	(*i)++;
	if (quote_type == 1)
		quote_char = '\'';
	else
		quote_char = '\"';
	while (word[*i] && word[*i] != quote_char)
		(*i)++;
	if (word[*i])
		(*i)++;
	final = ft_substr(word, start, *i - start);
	return (final);
}

char	*construct_tmp(char *word, int *i)
{
	char	*tmp;

	if (word[*i] == '\'')
		tmp = lex_quotes(1, i, word);
	else
		tmp = lex_quotes(2, i, word);
	return (tmp);
}

void	check_quotes(int *i, t_lexing **lexed, char *word)
{
	char	*tmp;
	char	*tmp2;
	char	*final;
	int		start;

	final = ft_strdup("");
	while (word[*i] && !ft_strchr("|&<>()", word[*i]))
	{
		if (word[*i] == '\'' || word[*i] == '\"')
			tmp = construct_tmp(word, i);
		else
		{
			start = *i;
			while (word[*i] && !ft_strchr("|&<>()'\"", word[*i]))
				(*i)++;
			tmp = ft_substr(word, start, *i - start);
		}
		tmp2 = final;
		final = ft_strjoin(final, tmp);
		free(tmp2);
		free(tmp);
	}
	add_token(lexed, final, "command", -1);
	free(final);
}
