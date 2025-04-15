/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_reds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:06:09 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 14:43:24 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	util_infile(t_lexing *tmp, t_gen *gen)
{
	t_lexing	*command;

	command = find_prev_command(gen->lexed_data, tmp);
	if (!command)
		command = find_next_node(tmp, "command");
	if (!command)
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	if (!access(tmp->value, F_OK))
		command->infile = open(tmp->value, O_RDONLY);
	else
		return (error_exit(gen, "minishell: no such file or directory", 1), 0);
	return (1);
}

int	util_outfile(t_lexing *tmp, t_gen *gen, t_lexing *redirect, t_lexing *lst)
{
	t_lexing	*command;

	command = find_prev_command(lst, tmp);
	if (!command)
		command = find_next_node(tmp, "command");
	if (!command)
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	redirect = find_next_node(command, "redirect_output");
	if (!redirect)
		command->outfile = open(tmp->value,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		command->outfile = open(tmp->value,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	return (1);
}
