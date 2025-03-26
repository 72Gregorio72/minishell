/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layerize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:12:41 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/26 14:31:23 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_close(t_lexing *node, t_lexing *succ)
{
	if (!succ)
		return (1);
	if (!ft_strncmp(node->type, "option", 7)
		|| !ft_strncmp(node->type, "command", 8)
		|| !ft_strncmp(node->type, "argument", 9)
		|| !ft_strncmp(succ->type, "close_parenthesis", 18)
		|| !ft_strncmp(node->type, "outfile", 8))
		return (1);
	return (0);
}

int	unclosed_parenth(t_lexing *node)
{
	t_lexing	*tmp;

	tmp = node;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "close_parenthesis", 18))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	check_not_opened(t_lexing *end, t_lexing *head)
{
	t_lexing	*tmp;
	int			found;

	tmp = head;
	found = 0;
	while (tmp != end)
	{
		if (!ft_strncmp(tmp->type, "open_parenthesis", 17))
			found = 1;
		tmp = tmp->next;
	}
	return (found);
}

int	check_parenthesis(t_lexing *lst, t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*succ;

	tmp = lst;
	succ = NULL;
	while (tmp)
	{
		if (tmp->next)
			succ = tmp->next;
		if (!ft_strncmp(tmp->type, "close_parenthesis", 18)
			&& !check_not_opened(tmp, lst))
			return (error_exit(gen, "minishell: syntax error", 2), 0);
		if (!ft_strncmp(succ->type, "close_parenthesis", 18) && !check_close(tmp, succ))
			return (error_exit(gen, "minishell: syntax error", 2), 0);
		if (!ft_strncmp(tmp->type, "open_parenthesis", 17))
		{
			if (!unclosed_parenth(succ))
				return (error_exit(gen, "minishell: syntax error", 2), 0);
			if (check_not_command(succ)
				|| !ft_strncmp(succ->type, "argument", 9)
				|| !ft_strncmp(succ->type, "option", 7))
				return (error_exit(gen, "minishell: syntax error", 2), 0);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	layerize(t_gen *gen)
{
	int			i;
	t_lexing	*tmp;

	i = 0;
	tmp = gen->lexed_data;
	if (!check_parenthesis(tmp, gen))
		return (0);
	while (tmp)
	{
		if (ft_strncmp(tmp->type, "open_parenthesis", 17) == 0)
			i++;
		else if (ft_strncmp(tmp->type, "close_parenthesis", 18) == 0)
			i--;
		if (i < 0)
			return (error_exit(gen, "minishell: syntax error", 2), 0);
		tmp->layer = i;
		tmp = tmp->next;
	}
	if (i != 0)
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	return (1);
}
