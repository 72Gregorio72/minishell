/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 10:43:19 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/06 16:46:58 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_lexing **lexed, char *content, char *type, int strength)
{
	if (content && *content)
		ft_lstadd_back(lexed, ft_lstnew(ft_strdup(content), type, strength));
}

static void	util_tokenize(char *word, int *i, t_lexing **lexed, int *start)
{
	check_quotes(i, lexed, word);
	other_checks_1(i, lexed, word);
	while (word[*i] == ' ')
		(*i)++;
	*start = *i;
	while (word[*i] && word[*i] != ' ' && !ft_strchr("|&<>()", word[*i]))
		(*i)++;
}

int	tokenize(char *word, t_lexing **lexed, t_gen *gen)
{
	int		i;
	int		start;
	char	*sub;

	i = 0;
	while (word[i])
	{
		util_tokenize(word, &i, lexed, &start);
		if (i > start)
		{
			sub = ft_substr(word, start, i - start);
			add_token(lexed, sub, "command", 1);
			free(sub);
			continue ;
		}
		if (word[i] == '&')
			return (error_exit(gen, "minishell: syntax error near '&'", 2),
				ft_lstclear(*lexed, 0), 0);
		if (!word[i])
			break ;
		if (!other_checks(&i, lexed, word, gen))
			return (0);
		check_pipe(&i, lexed, word);
	}
	return (1);
}

void	find_prevs(t_lexing *lexed)
{
	t_lexing	*tmp;

	tmp = lexed;
	while (tmp)
	{
		if (tmp->next)
			tmp->next->prev = tmp;
		tmp = tmp->next;
	}
}

t_lexing	*lexer(char **matrix, t_gen *gen)
{
	int			i;
	t_lexing	*lexed;

	i = 0;
	lexed = NULL;
	(void)gen;
	while (matrix[i])
	{
		if (!tokenize(matrix[i], &lexed, gen))
			return (NULL);
		i++;
	}
	find_env_var_and_wild(lexed);
	find_prevs(lexed);
	return (lexed);
}
