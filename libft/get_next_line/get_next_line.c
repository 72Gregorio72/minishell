/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:47:29 by vcastald          #+#    #+#             */
/*   Updated: 2025/01/27 12:39:49 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*find_newline(char **str)
{
	char	*fin;
	char	*tmp;
	int		i;

	i = 0;
	while ((*str)[i] && (*str)[i] != '\n')
		i++;
	if ((*str)[i] == '\n')
	{
		fin = ft_substr(*str, 0, i + 1);
		tmp = ft_substr(*str, i + 1, ft_strlen(*str) - i - 1);
		free(*str);
		*str = tmp;
		if (!(*str) || ft_strlen(*str) == 0)
		{
			free(*str);
			*str = NULL;
		}
	}
	else
		fin = NULL;
	return (fin);
}

int	read_line(char **s, int fd)
{
	char	*buff;
	char	*tmp;
	int		bytes_read;

	buff = (char *)ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buff)
		return (-1);
	bytes_read = read(fd, buff, BUFFER_SIZE);
	if (bytes_read > 0)
	{
		tmp = ft_strjoin(*s, buff);
		free(*s);
		*s = tmp;
	}
	free(buff);
	return (bytes_read);
}

char	*test(char **leftovers, char **final, int bytes)
{
	if (bytes < 0)
	{
		free(*leftovers);
		*leftovers = NULL;
		return (NULL);
	}
	else if (bytes == 0)
	{
		if (*leftovers && ft_strlen(*leftovers) > 0)
		{
			*final = *leftovers;
			*leftovers = NULL;
			return (*final);
		}
		free(*leftovers);
		*leftovers = NULL;
		return (NULL);
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*leftovers;
	char		*final;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!leftovers)
		leftovers = (ft_strdup(""));
	if (!leftovers)
		return (NULL);
	while (1)
	{
		final = find_newline(&leftovers);
		if (final)
			return (final);
		bytes = read_line(&leftovers, fd);
		if (bytes <= 0)
			return (test(&leftovers, &final, bytes));
	}
}
/* 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "get_next_line.h"

int main(int argc, char **argv)
{
    int fd1;
    char *line1;   if (argc != 2) // Due file richiesti come input
    {
        fprintf(stderr, "Usage: %s <file1>\n", argv[0]);
        return (EXIT_FAILURE);
    }    // Apertura del primo file
    fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0)
    {
        perror("Error opening file 1");
        return (EXIT_FAILURE);
    }    // Leggi alternativamente dai due file
    while (1)
    {
        line1 = get_next_line(fd1);
        if (line1 != NULL)
        {
            printf("File1: %s\n", line1);
            free(line1);
        }        
        if (line1 == NULL)
            break;
    }    close(fd1);
    return (EXIT_SUCCESS);
} */
