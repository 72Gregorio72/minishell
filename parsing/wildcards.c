/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:49:27 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/04 14:27:53 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_wildcard(t_lexing **node)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*tmp;
	char			*tmp2;

	if (!ft_strchr((*node)->value, '.'))
	{
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
		entry = readdir(dir);
		while (entry)
		{
			if (entry->d_name[0] != '.')
			{
				tmp2 = ft_strjoin((*node)->value, entry->d_name);
				free((*node)->value);
				(*node)->value = tmp2;
				tmp2 = ft_strjoin((*node)->value, " ");
				free((*node)->value);
				(*node)->value = tmp2;
			}
			entry = readdir(dir);
		}
		closedir(dir);
		free(tmp);
	}
	return (1);
}

int	check_wildcards(t_gen *gen)
{
	t_lexing	*tmp;

	tmp = gen->lexed_data;
	while (tmp)
	{
		if (tmp->wildcard
			&& ft_strncmp(tmp->type, "here_doc_delimiter", 19) != 0)
		{
			if (!ft_strncmp(tmp->type, "outfile", 8)
				|| !ft_strncmp(tmp->type, "infile", 7))
				return (error_exit(gen, "minishell: ambiguos redirect", 1), 0);
			else
				expand_wildcard(&tmp);
		}
		tmp = tmp->next;
	}
	return (1);
}
