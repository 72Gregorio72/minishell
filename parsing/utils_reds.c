/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_reds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:06:09 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/23 14:04:47 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	util_infile(t_lexing *tmp, t_gen *gen, t_lexing *lst)
{
	t_lexing	*command;

	command = find_prev_command(lst, tmp);
	if (!command)
		command = find_next_node(tmp, "command");
	if (!command)
		command = tmp;
	if (!access(tmp->value, F_OK))
		command->infile = open(tmp->value, O_RDONLY);
	else
		return (error_exit(gen, "minishell: no such file or directory", 1), 0);
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
	if (!redirect)
		command->outfile = open(tmp->value,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		command->outfile = open(tmp->value,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	return (1);
}
