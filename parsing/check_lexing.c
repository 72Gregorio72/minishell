/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_lexing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:19:27 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/08 10:02:36 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	other_checks(int *i, t_lexing **lexed, char *word, t_gen *gen)
{
	if (ft_strncmp(&word[*i], "(", ft_strlen("(")) == 0)
	{
		add_token(lexed, "(", "open_parenthesis", -1);
		*i += ft_strlen("(");
	}
	else if (ft_strncmp(&word[*i], ")", ft_strlen(")")) == 0)
	{
		add_token(lexed, ")", "close_parenthesis", -1);
		if (word[(*i) + 1])
			return (error_exit(gen, "minishell: syntax error", 2),
				ft_lstclear(*lexed), 0);
		*i += ft_strlen(")");
	}
	else if (ft_strncmp(&word[*i], "||", ft_strlen("||")) == 0)
	{
		add_token(lexed, "||", "or_operator", 3);
		*i += ft_strlen("||");
	}
	return (1);
}

void	other_checks_1(int *i, t_lexing **lexed, char *word)
{
	if (ft_strncmp(&word[*i], "&&", ft_strlen("&&")) == 0)
	{
		add_token(lexed, "&&", "and_operator", 3);
		*i += ft_strlen("&&");
	}
	else if (ft_strncmp(&word[*i], "<<", ft_strlen("<<")) == 0)
	{
		add_token(lexed, "<<", "here_doc", 1);
		*i += ft_strlen("<<");
	}
	else if (ft_strncmp(&word[*i], ">>", ft_strlen(">>")) == 0)
	{
		add_token(lexed, ">>", "output_append", 1);
		*i += ft_strlen(">>");
	}
	else if (ft_strncmp(&word[*i], "<", ft_strlen("<")) == 0)
	{
		add_token(lexed, "<", "redirect_input", 1);
		*i += ft_strlen("<");
	}
	else if (ft_strncmp(&word[*i], ">", ft_strlen(">")) == 0)
	{
		add_token(lexed, ">", "redirect_output", 1);
		*i += ft_strlen(">");
	}
}

void	check_pipe(int *i, t_lexing **lexed, char *word)
{
	if (ft_strncmp(&word[*i], "|", ft_strlen("|")) == 0)
	{
		add_token(lexed, "|", "pipe", 2);
		*i += ft_strlen("|");
	}
}

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
		{
			if (word[*i] == '\'')
				tmp = lex_quotes(1, i, word);
			else
				tmp = lex_quotes(2, i, word);
		}
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
