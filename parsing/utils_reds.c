/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_reds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:06:09 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/06 16:24:12 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

int	util_infile(char *filename, t_gen *gen, t_lexing *node)
{
	if (is_directory(filename))
		return (error_exit(gen, "minishell: path is a directory", 1), 2);
	if (!access(filename, F_OK))
	{
		node->infile = open(filename, O_RDONLY);
		if (node->infile < 0)
			return (safe_free(gen), perror("open error"), exit(1), 0);
	}
	else
		return (error_exit(gen, "minishell: no such file or directory", 1), 2);
	return (1);
}

t_lexing	*find_prev_redirect(t_lexing *start, t_lexing *end)
{
	t_lexing	*tmp;

	tmp = start;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "redirect_output", 16)
			&& tmp->next && tmp->next == end)
			return (tmp);
		if (tmp == end)
			break ;
		if (tmp && tmp->next)
			tmp = tmp->next;
	}
	return (NULL);
}

int	util_outfile(char *filename, t_gen *gen, t_lexing *node, int flag)
{
	if (is_directory(filename))
		return (error_exit(gen, "minishell: path is a directory", 1), 2);
	if (flag == 2)
		node->outfile = open(filename,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else if (flag == 1)
		node->outfile = open(filename,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (node->outfile < 0)
		return (safe_free(gen), perror("open error"), exit(1), 0);
	return (1);
}
