<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_lexing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:23:20 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/08 11:09:10 by vcastald         ###   ########.fr       */
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
		if (tmp->next)
			succ = tmp->next;
		if (!ft_strncmp(tmp->type, "here_doc", 9) && tmp->next
			&& !ft_strncmp(succ->type, "command", 8))
			succ->type = ft_strdup("here_doc_delimiter");
		if (!ft_strncmp(tmp->type, "command", 8) && tmp->next)
		{
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

void	find_env_var_and_wild(t_lexing *lexed)
{
	t_lexing	*tmp;
	int			dollar_pos;

	tmp = lexed;
	while (tmp)
	{
		dollar_pos = find_char_pos(tmp->value, "$", 0);
		if (ft_strchr(tmp->value, '$') != NULL
			&& !ft_isdigit(tmp->value[dollar_pos + 1]))
			tmp->env_variable = 1;
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

int	count_nodes_before(t_lexing *end, t_lexing *start)
{
	int			count;
	t_lexing	*tmp;

	count = 0;
	tmp = start;
	while (tmp)
	{
		if (!ft_strncmp(tmp->value, end->value, ft_strlen(end->value)))
			return (count);
		count++;
		tmp = tmp->next;
	}
	return (count);
}

int	check_files(t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = gen->lexed_data;
	succ = NULL;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if (succ && !ft_strncmp(tmp->type, "redirect_input", 15)
			&& check_not_command(succ))
			return (error_exit(gen, "mininshell: syntax error", 2), 0);
		if (succ
			&& (!ft_strncmp(tmp->type, "redirect_output", 16)
				|| !ft_strncmp(tmp->type, "output_append", 14))
			&& check_not_command(succ))
			return (error_exit(gen, "mininshell: syntax error", 2), 0);
		tmp = tmp->next;
	}
	return (1);
}
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_lexing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:23:20 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/09 12:45:15 by vcastald         ###   ########.fr       */
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
		if (tmp->next)
			succ = tmp->next;
		if (!ft_strncmp(tmp->type, "here_doc", 9) && tmp->next
			&& !ft_strncmp(succ->type, "command", 8))
			succ->type = ft_strdup("here_doc_delimiter");
		if (!ft_strncmp(tmp->type, "command", 8) && tmp->next)
		{
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

void	find_env_var_and_wild(t_lexing *lexed)
{
	t_lexing	*tmp;
	int			dollar_pos;

	tmp = lexed;
	while (tmp)
	{
		dollar_pos = find_char_pos(tmp->value, "$", 0);
		if (ft_strchr(tmp->value, '$') != NULL)
		{
			if (!(ft_strlen(tmp->value) == 1 && tmp->value[0] == '$'))
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

/* int	count_nodes_before(t_lexing *end, t_lexing *start)
{
	int			count;
	t_lexing	*tmp;

	count = 0;
	tmp = start;
	while (tmp)
	{
		if (!ft_strncmp(tmp->value, end->value, ft_strlen(end->value)))
			return (count);
		count++;
		tmp = tmp->next;
	}
	return (count);
} */

int	check_operators(t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*last;

	tmp = gen->lexed_data;
	last = ft_lstlast(tmp);
	if (!ft_strncmp("and_operator", last->type, 13))
		return (error_exit(gen, "minishell: syntax error near '&&'", 2), 0);
	else if (!ft_strncmp("or_operator", last->type, 12))
		return (error_exit(gen, "minishell: syntax error near '||'", 2), 0);
	else if (!ft_strncmp("pipe", last->type, 12))
		return (error_exit(gen, "minishell: syntax error near '|'", 2), 0);
	return (1);
}

int	check_files(t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = gen->lexed_data;
	succ = NULL;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if (succ && !ft_strncmp(tmp->type, "redirect_input", 15)
			&& check_not_command(succ))
			return (error_exit(gen, "mininshell: syntax error", 2), 0);
		if (succ
			&& (!ft_strncmp(tmp->type, "redirect_output", 16)
				|| !ft_strncmp(tmp->type, "output_append", 14))
			&& check_not_command(succ))
			return (error_exit(gen, "mininshell: syntax error", 2), 0);
		tmp = tmp->next;
	}
	return (1);
}
>>>>>>> origin/vcastald
