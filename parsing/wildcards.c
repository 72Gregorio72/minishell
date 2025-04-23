/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:49:27 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/23 09:32:10 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_peek(DIR *dir, char *tmp2, t_lexing **node, struct dirent *peek)
{
	while (peek && peek->d_name[0] == '.')
		peek = readdir(dir);
	if (peek)
	{
		tmp2 = ft_strjoin((*node)->value, " ");
		free((*node)->value);
		(*node)->value = tmp2;
	}
}

void	construct_val(t_lexing **node, struct dirent **entry, DIR *dir)
{
	struct dirent	*peek;
	char			*tmp2;

	tmp2 = ft_strjoin((*node)->value, (*entry)->d_name);
	free((*node)->value);
	(*node)->value = tmp2;
	peek = readdir(dir);
	check_peek(dir, tmp2, node, peek);
	(*entry) = peek;
}

int	match_wildcard(const char *str, const char *pattern)
{
	if (!pattern || !str)
		return (0);
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
		return (match_wildcard(str, pattern + 1)
			|| (*str && match_wildcard(str + 1, pattern)));
	if (*pattern == *str)
		return (match_wildcard(str + 1, pattern + 1));
	return (0);
}

void	loop_wild(struct dirent *entry, DIR *dir, t_lexing **node, char *wild)
{
	entry = readdir(dir);
	while (entry)
	{
		if (!ft_strncmp(wild, "*", ft_strlen(wild)))
		{
			if (entry->d_name[0] != '.' && wild[0] != '.')
			{
				construct_val(node, &entry, dir);
				continue ;
			}
		}
		else
		{
			if (match_wildcard(entry->d_name, wild))
				construct_val(node, &entry, dir);
		}
		entry = readdir(dir);
	}
}

int	expand_wildcard(t_lexing **node, t_gen *gen)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*tmp;

	dir = opendir(".");
	entry = NULL;
	if (!dir)
		return (perror("opendir"), 0);
	tmp = ft_strdup((*node)->value);
	if (!tmp)
		return (closedir(dir), 0);
	free((*node)->value);
	(*node)->value = ft_strdup("");
	if (!(*node)->value)
		return (closedir(dir), free(tmp), 0);
	loop_wild(entry, dir, node, tmp);
	if ((*node)->value[0] == '\0')
	{
		free((*node)->value);
		(*node)->value = ft_strdup(tmp);
	}
	sort_value(node, gen);
	return (closedir(dir), free(tmp), 1);
}
