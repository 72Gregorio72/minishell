/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:49:27 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 10:01:49 by vcastald         ###   ########.fr       */
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

int	not_dot(struct dirent **entry, DIR *dir, char *tmp, t_lexing **node)
{
	char			*dot_pos;
	char			*dot_pos_wild;

	if ((*entry)->d_name[0] != '.')
	{
		dot_pos = ft_strrchr((*entry)->d_name, '.');
		dot_pos_wild = ft_strrchr(tmp, '.');
		if (dot_pos && dot_pos_wild
			&& !ft_strncmp(dot_pos, dot_pos_wild, ft_strlen(dot_pos_wild)))
		{
			construct_val(node, entry, dir);
			return (1);
		}
	}
	return (0);
}

void	loop_wild(struct dirent *entry, DIR *dir, t_lexing **node, char *tmp)
{
	int				dot_pres;

	if (!ft_strchr(tmp, '.'))
		dot_pres = 0;
	else
		dot_pres = 1;
	entry = readdir(dir);
	while (entry)
	{
		if (!dot_pres)
		{
			if (entry->d_name[0] != '.')
			{
				construct_val(node, &entry, dir);
				continue ;
			}
		}
		else
		{
			if (not_dot(&entry, dir, tmp, node))
				continue ;
		}
		entry = readdir(dir);
	}
}

int	expand_wildcard(t_lexing **node)
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
	closedir(dir);
	free(tmp);
	return (1);
}
