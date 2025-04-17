/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_wild.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 09:29:29 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/17 10:25:07 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

int	compare_words(const char *a, const char *b)
{
	int				i;
	unsigned char	ca;
	unsigned char	cb;

	i = 0;
	while (a[i] && b[i])
	{
		ca = ft_tolower(a[i]);
		cb = ft_tolower(b[i]);

		if (ca != cb)
			return (ca - cb);
		if (a[i] != b[i])
			return (a[i] - b[i]);

		i++;
	}
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

void	sort_words(char **input_words)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (input_words[i])
	{
		j = i + 1;
		while (input_words[j])
		{
			if (compare_words(input_words[i], input_words[j]) > 0)
			{
				tmp = input_words[i];
				input_words[i] = input_words[j];
				input_words[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	reconstruct_sorted_value(char *input, char **input_words)
{
	int	i;
	int	pos;
	int	j;

	i = 0;
	pos = 0;
	while (input_words[i])
	{
		j = 0;
		while (input_words[i][j])
			input[pos++] = input_words[i][j++];
		if (input_words[i + 1])
			input[pos++] = ' ';
		i++;
	}
	input[pos] = '\0';
}

void	sort_value(t_lexing **node, t_gen *gen)
{
	char	**words;

	words = ft_split_quote((*node)->value, ' ');
	if (!words)
		return (safe_free(gen), perror("malloc error"), exit(1));
	sort_words(words);
	reconstruct_sorted_value((*node)->value, words);
	free_matrix(words);
}
