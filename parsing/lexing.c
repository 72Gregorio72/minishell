/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 10:43:19 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/26 12:30:27 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_lexing **lexed, char *content, char *type, int strength)
{
	if (content && *content)
		ft_lstadd_back(lexed, ft_lstnew(ft_strdup(content), type, strength));
}

void	tokenize(char *word, t_lexing **lexed)
{
	int		i;
	int		start;
	char	*sub;

	i = 0;
	while (word[i])
	{
		check_quotes(&i, lexed, word);
		while (word[i] == ' ')
			i++;
		start = i;
		while (word[i] && word[i] != ' ' && !ft_strchr("|&<>()", word[i]))
			i++;
		if (i > start)
		{
			sub = ft_substr(word, start, i - start);
			add_token(lexed, sub, "command", 1);
			free(sub);
		}
		if (!word[i])
			break ;
		other_checks(&i, lexed, word);
		other_checks_1(&i, lexed, word);
		check_pipe(&i, lexed, word);
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
		tokenize(matrix[i], &lexed);
		i++;
	}
	find_args(lexed);
	find_env_var(lexed);
	return (lexed);
}
// ( < echo 1 > || << echo 2 >> ) | ( echo '3 ' && echo " 4 " )