/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_lexing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:23:20 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/16 09:21:57 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	util_args(t_lexing *tmp)
{
	if (!ft_strncmp(tmp->type, "here_doc", 9) && tmp->next
		&& !ft_strncmp(tmp->next->type, "command", 8))
		tmp->next->type = ft_strdup("here_doc_delimiter");
	else if (!ft_strncmp(tmp->type, "command", 8))
	{
		if (tmp->value[0] == '-')
			tmp->type = ft_strdup("option");
		else
			tmp->type = ft_strdup("argument");
	}
}

void	find_args(t_lexing *lexed)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lexed;
	succ = NULL;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if (!ft_strncmp(tmp->type, "here_doc", 9) && tmp->next
			&& !ft_strncmp(succ->type, "command", 8))
			succ->type = ft_strdup("here_doc_delimiter");
		else if (!ft_strncmp(tmp->type, "command", 8))
		{
			tmp = tmp->next;
			while (tmp && !check_not_command(tmp))
			{
				util_args(tmp);
				tmp = tmp->next;
			}
		}
		if (tmp)
			tmp = tmp->next;
	}
}

void	find_env_var_and_wild(t_lexing *lexed)
{
	t_lexing	*tmp;
	t_lexing	*command;
	int			dollar_pos;

	tmp = lexed;
	while (tmp)
	{
		command = find_prev_command(lexed, tmp);
		dollar_pos = find_char_pos(tmp->value, "$", 0);
		if (ft_strchr(tmp->value, '$') != NULL
			&& ft_strncmp(command->value, "export",
				ft_strlen(command->value)) != 0)
		{
			if (!(ft_strlen(tmp->value) == 1 && dollar_pos == 0))
				tmp->env_variable = 1;
		}
		if (ft_strchr(tmp->value, '*') != NULL
			&& (!ft_strchr(tmp->value, '\"') && !ft_strchr(tmp->value, '\'')))
			tmp->wildcard = 1;
		tmp = tmp->next;
	}
}

int	find_files(t_lexing *lexed, t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lexed;
	succ = lexed;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if ((!ft_strncmp("output_append", tmp->type, 14)
				|| !ft_strncmp("redirect_output", tmp->type, 16)))
		{
			if (!ft_strncmp("command", succ->type, 8))
				succ->type = ft_strdup("outfile");
			else if (ft_strncmp("command", succ->type, 8) != 0 || !tmp->next)
				return (error_exit(gen, "minishell: syntax error", 2), 0);
		}
		else if (!ft_strncmp("redirect_input", tmp->type, 15))
		{
			if (!ft_strncmp("command", succ->type, 9))
				succ->type = ft_strdup("infile");
		}
		tmp = tmp->next;
	}
	return (1);
}

int	check_operators(t_gen *gen, t_lexing *lexed)
{
	t_lexing	*tmp;
	t_lexing	*last;

	tmp = lexed;
	if (!ft_strncmp("and_operator", tmp->type, 13))
		return (error_exit(gen, "minishell: syntax error near '&&'", 2), 0);
	else if (!ft_strncmp("or_operator", tmp->type, 12))
		return (error_exit(gen, "minishell: syntax error near '||'", 2), 0);
	else if (!ft_strncmp("pipe", tmp->type, 12))
		return (error_exit(gen, "minishell: syntax error near '|'", 2), 0);
	last = ft_lstlast(tmp);
	if (!ft_strncmp("and_operator", last->type, 13))
		return (error_exit(gen, "minishell: syntax error near '&&'", 2), 0);
	else if (!ft_strncmp("or_operator", last->type, 12))
		return (error_exit(gen, "minishell: syntax error near '||'", 2), 0);
	else if (!ft_strncmp("pipe", last->type, 12))
		return (error_exit(gen, "minishell: syntax error near '|'", 2), 0);
	return (1);
}
