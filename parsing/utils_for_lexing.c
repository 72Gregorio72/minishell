/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_lexing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:23:20 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/26 12:29:53 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_args(t_lexing *lexed)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lexed;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "command", 8))
		{
			succ = tmp->next;
			while (succ && succ->type && !ft_strncmp(succ->type, "command", 8))
			{
				if (check_not_command(succ))
					break ;
				if (succ->value[0] == '-')
					succ->type = ft_strdup("option");
				else
					succ->type = ft_strdup("argument");
				succ = succ->next;
			}
		}
		tmp = tmp->next;
	}
}

void	find_env_var(t_lexing *lexed)
{
	t_lexing	*tmp;

	tmp = lexed;
	while (tmp)
	{
		if (check_all_upper(tmp->value) && ft_strchr(tmp->value, '$') != NULL)
			tmp->env_variable = 1;
		tmp = tmp->next;
	}
}

int	find_files(t_lexing *lexed, t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lexed;
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
		else if (!ft_strncmp("command", tmp->type, 8))
		{
			if (!ft_strncmp("redirect_input", succ->type, 15))
				tmp->type = ft_strdup("infile");
		}
		tmp = tmp->next;
	}
	return (1);
}
