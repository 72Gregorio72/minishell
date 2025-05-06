/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_reds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:06:09 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/06 11:51:02 by vcastald         ###   ########.fr       */
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

int	util_infile(t_lexing *tmp, t_gen *gen, t_lexing *lst)
{
	t_lexing	*command;

	command = find_prev_command(lst, tmp);
	if (!command)
		command = find_next_node(tmp, "command");
	if (!command)
		command = tmp;
	if (is_directory(tmp->value))
		return (error_exit(gen, "minishell: path is a directory", 1), 0);
	if (!access(tmp->value, F_OK))
	{
		command->infile = open(tmp->value, O_RDONLY);
		if (command->infile < 0)
			return (safe_free(gen), perror("open error"), exit(1), 0);
	}
	else
		return (error_exit(gen, "minishell: no such file or directory", 1), 2);
	return (1);
}

int	util_outfile(t_lexing *tmp, t_gen *gen, t_lexing *redirect, t_lexing *lst)
{
	t_lexing	*command;

	(void)gen;
	command = find_prev_command(lst, tmp);
	if (!command)
		command = find_next_node(tmp, "command");
	if (!command)
		command = tmp;
	redirect = find_next_node(command, "redirect_output");
	if (is_directory(tmp->value))
		return (error_exit(gen, "minishell: path is a directory", 1), 0);
	if (!redirect)
		command->outfile = open(tmp->value,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		command->outfile = open(tmp->value,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (command->outfile < 0)
		return (safe_free(gen), perror("open error"), exit(1), 0);
	return (1);
}
