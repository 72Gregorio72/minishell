/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_quotes2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:22:26 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/06 09:32:24 by vcastald         ###   ########.fr       */
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

char	*construct_env_var(char *before, char *after, char *tmp)
{
	char	*new_value;

	new_value = (char *)malloc(ft_strlen(before)
			+ ft_strlen(tmp) + ft_strlen(after) + 1);
	if (!new_value)
		return (NULL);
	new_value[0] = '\0';
	ft_strlcat(new_value, before, ft_strlen(before) + 1);
	ft_strlcat(new_value, tmp, ft_strlen(new_value) + ft_strlen(tmp) + 1);
	ft_strlcat(new_value, after, ft_strlen(new_value) + ft_strlen(after) + 1);
	return (new_value);
}

int	double_quotes(int *i, t_lexing **node, t_gen *gen)
{
	(*i)++;
	while ((*node)->value[*i] && (*node)->value[*i] != '\"')
	{
		if ((*node)->value[*i] == '$')
			handle_env_variable(node, gen, i);
		(*i)++;
	}
	if ((*node)->value[*i])
		(*i)++;
	(*node)->expanded = 0;
	return (1);
}
