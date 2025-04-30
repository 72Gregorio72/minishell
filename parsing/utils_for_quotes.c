/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:52:28 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/30 10:19:19 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_with_num(char **tmp, t_gen *gen, int *e)
{
	*tmp = ft_strdup("");
	if (!(*tmp))
	{
		perror("malloc error");
		safe_free(gen);
		exit(gen->exit_status);
	}
	*e = 1;
}

static void	util_tmp(char **tmp, int *e)
{
	*tmp = ft_strdup("$");
	*e = 1;
}

char	*build_tmp(t_gen *gen, int *e, char **value, int p)
{
	char	*tmp;

	if (!(*value)[p + 1])
		util_tmp(&tmp, e);
	else if (ft_isdigit((*value)[p + 1]))
		start_with_num(&tmp, gen, e);
	else if ((*value)[p + 1] == '?')
	{
		tmp = ft_itoa(gen->exit_status);
		if (!tmp)
			return (safe_free(gen), exit(gen->exit_status), NULL);
		*e = ft_strlen(tmp);
	}
	else
	{
		tmp = expand_env_var(gen->my_env, *value);
		if (!tmp)
		{
			tmp = ft_strdup("");
			if (!tmp)
				return (safe_free(gen), exit(gen->exit_status), NULL);
		}
		*e = len_var(*value, p);
	}
	return (tmp);
}

void	util_exit(t_gen *gen)
{
	safe_free(gen);
	perror("malloc error");
	exit(gen->exit_status);
}

// p = doll_pos
// n = node
// l = len
void	handle_env_variable(t_lexing **n, t_gen *gen, int *p)
{
	char	*tmp;
	char	*before;
	char	*after;
	int		l;
	int		len_var_expanded;

	len_var_expanded = 0;
	tmp = NULL;
	after = NULL;
	before = NULL;
	l = 0;
	before = ft_substr((*n)->value, 0, *p);
	if (!before)
		return (util_exit(gen));
	tmp = build_tmp(gen, &l, &(*n)->value, *p);
	len_var_expanded = ft_strlen(tmp);
	after = ft_substr((*n)->value, (*p) + l + 1, ft_strlen((*n)->value));
	if (!before)
		return (free(before), free(tmp), util_exit(gen));
	free((*n)->value);
	(*n)->value = construct_env_var(before, after, tmp);
	if (!(*n)->value)
		return (util_free_env_var(before, after, tmp), util_exit(gen));
	util_free_env_var(before, tmp, after);
	(*p) += len_var_expanded - 1;
}
