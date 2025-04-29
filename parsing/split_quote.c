/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:21:02 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/29 12:57:31 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

static char	*ft_strndup(const char *src, int len)
{
	char	*dest;
	int		i;

	dest = (char *)malloc((len + 1) * sizeof(char));
	if (!dest)
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

static int	parse_next_token(const char *str, char delim, int *index)
{
	int		start;
	char	quote;

	while (str[*index] && str[*index] == delim)
		(*index)++;
	start = *index;
	quote = 0;
	while (str[*index])
	{
		if ((str[*index] == '\'' || str[*index] == '"')
			&& (quote == 0 || quote == str[*index]))
		{
			if (quote == 0)
				quote = str[*index];
			else
				quote = 0;
		}
		else if (str[*index] == delim && quote == 0)
			break ;
		(*index)++;
	}
	return (start);
}

static int	count_words(const char *str, char delim)
{
	int		index;
	int		count;
	int		start;

	index = 0;
	count = 0;
	while (str[index])
	{
		start = parse_next_token(str, delim, &index);
		if (index > start)
			count++;
	}
	return (count);
}

static char	**split_tokens(const char *str, char delim, int count)
{
	char	**final;
	int		index;
	int		row;
	int		start;

	index = 0;
	row = 0;
	final = (char **)malloc((count + 1) * sizeof(char *));
	if (!final)
		return (NULL);
	while (str[index])
	{
		start = parse_next_token(str, delim, &index);
		if (index > start)
		{
			final[row] = ft_strndup(str + start, index - start);
			if (!final[row])
				return (free_final(final, row), NULL);
			row++;
		}
	}
	final[row] = NULL;
	return (final);
}

char	**ft_split_quote(const char *s, char c)
{
	int		word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	return (split_tokens(s, c, word_count));
}


/* #include <stdio.h>

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
} */
