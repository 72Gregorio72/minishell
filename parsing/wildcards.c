/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:49:27 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/23 10:24:19 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	construct_val(t_lexing **node, struct dirent **entry, int add_space,
					t_gen *gen)
{
	char	*tmp;
	char	*tmp2;

	if (add_space)
	{
		tmp = ft_strjoin((*node)->value, " ");
		if (!tmp)
			return (safe_free(gen), exit(1));
		tmp2 = ft_strjoin(tmp, (*entry)->d_name);
		free(tmp);
	}
	else
		tmp2 = ft_strjoin((*node)->value, (*entry)->d_name);
	if (!tmp2)
		return (safe_free(gen), exit(1));
	free((*node)->value);
	(*node)->value = tmp2;
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

void	loop_wild(DIR *dir, t_lexing **node, char *wild, t_gen *gen)
{
	struct dirent	*entry;
	int				first;

	first = 1;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.' && wild[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (!ft_strncmp(wild, "*", ft_strlen(wild))
			|| match_wildcard(entry->d_name, wild))
		{
			construct_val(node, &entry, !first, gen);
			first = 0;
		}
		entry = readdir(dir);
	}
}

int	expand_wildcard(t_lexing **node, t_gen *gen)
{
	DIR				*dir;
	char			*tmp;

	dir = opendir(".");
	if (!dir)
		return (perror("opendir"), 0);
	tmp = ft_strdup((*node)->value);
	if (!tmp)
		return (closedir(dir), 0);
	free((*node)->value);
	(*node)->value = ft_strdup("");
	if (!(*node)->value)
		return (closedir(dir), free(tmp), 0);
	loop_wild(dir, node, tmp, gen);
	if ((*node)->value[0] == '\0')
	{
		free((*node)->value);
		(*node)->value = ft_strdup(tmp);
	}
	sort_value(node, gen);
	return (closedir(dir), free(tmp), 1);
}
