/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 10:43:19 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/24 14:12:44 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_args(t_lexing *lexed)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lexed;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "command", 8))
		{
			succ = tmp->next;
			while (succ && succ->type && !ft_strncmp(succ->type, "command", 8))
			{
				if (check_not_command(succ))
					break ;
				if (succ->value[0] == '-')
					succ->type = ft_strdup("option");
				else
				{
					succ->type = ft_strdup("argument");
					break ;
				}
				succ = succ->next;
			}
		}
		tmp = tmp->next;
	}
}

void	find_env_var(t_lexing *lexed)
{
	t_lexing	*tmp;

	tmp = lexed;
	while (tmp)
	{
		if (check_all_upper(tmp->value) && ft_strchr(tmp->value, '$') != NULL)
			tmp->env_variable = 1;
		tmp = tmp->next;
	}
}

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

t_lexing	*lexer(char **matrix)
{
	int			i;
	t_lexing	*lexed;

	i = 0;
	lexed = NULL;
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