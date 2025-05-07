/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_reds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:06:09 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/07 11:06:09 by vcastald         ###   ########.fr       */
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

int	calc_mat_len(t_lexing *node, int *i)
{
	int	mat_length;

	mat_length = 0;
	while (node->command[*i])
	{
		if (ft_strncmp(node->command[*i], "<", 1)
			&& ft_strncmp(node->command[*i], ">>", 2)
			&& ft_strncmp(node->command[*i], ">", 1))
			mat_length++;
		else
			(*i)++;
		(*i)++;
	}
	*i = 0;
	return (mat_length);
}

int	check_here_doc(t_gen *gen, t_lexing *lexed)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lexed;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if (!ft_strncmp(tmp->type, "here_doc", 9)
			&& (!tmp->next
				|| ft_strncmp(succ->type, "here_doc_delimiter", 19) != 0))
			return (error_exit(gen, "minishell: syntax error", 2), 0);
		tmp = tmp->next;
	}
	return (1);
}
