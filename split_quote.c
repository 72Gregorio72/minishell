/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:21:02 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/21 16:22:56 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

static char	*ft_strndup(char const *src, int len)
{
	int		i;
	char	*dest;

	dest = (char *)malloc((len + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	n_words(char c, char const *str)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str[i])
	{
		if (!(str[i] == c) && !in_word)
		{
			count++;
			in_word = 1;
		}
		else if (str[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

int	free_final(char **final, int row)
{
	int	i;

	i = 0;
	while (i <= row)
	{
		free(final[i]);
		i++;
	}
	return (1);
}

static int	parse_next_token(char const *str, char c, int *index)
{
	int		start;
	int		in_quotes;

	in_quotes = 0;
	while (str[*index] && (str[*index] == c && !in_quotes))
		(*index)++;
	start = *index;
	while (str[*index] && (in_quotes || str[*index] != c))
	{
		if (str[*index] == '\'' || str[*index] == '"')
			in_quotes = !in_quotes;
		(*index)++;
	}
	return (start);
}

static char	**split(char **final, char const *str, char c, int freed)
{
	int		i;
	int		start;
	int		row;

	i = 0;
	row = 0;
	while (str[i])
	{
		start = parse_next_token(str, c, &i);
		if (i > start)
		{
			final[row] = ft_strndup(str + start, i - start);
			if (final[row] == NULL)
				freed = free_final(final, row);
			row++;
		}
	}
	if (!freed)
		final[row] = NULL;
	return (final);
}

char	**ft_split_quote(char const *s, char c)
{
	int		num_words;
	char	**final;

	num_words = n_words(c, s);
	final = (char **)malloc((num_words + 1) * sizeof(char *));
	if (final == NULL)
		return (NULL);
	return (split(final, s, c, 0));
}

#include <stdio.h>

int main(void) {
    char str[] = "Hello World '     42 ' 42 'sd'";
    char c = ' ';
    char **split = ft_split_quote(str, c);

    // Print the split result
    int i = 0;
    while (split[i])
    {
        printf("%s\n", split[i]);
        free(split[i]); // Don't forget to free the individual strings
        i++;
    }
    free(split); // Free the array of strings itself
    return 0;
}
