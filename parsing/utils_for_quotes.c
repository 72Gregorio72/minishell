/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:52:28 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/02 09:39:44 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*construct_env_var(char *before, char *after, char *tmp)
{
	char	*new_value;

	new_value = (char *)malloc(ft_strlen(before)
			+ ft_strlen(tmp) + ft_strlen(after) + 1);
	if (!new_value)
		return (NULL);
	new_value[0] = '\0';
	ft_strlcat(new_value, before, ft_strlen(before) + 1);
	ft_strlcat(new_value, tmp, ft_strlen(new_value) + ft_strlen(tmp) + 1);
	ft_strlcat(new_value, after, ft_strlen(new_value) + ft_strlen(after) + 1);
	return (new_value);
}

static void	util_free(char *before, char *tmp, char *after)
{
	free(before);
	free(tmp);
	free(after);
}

void	handle_env_variable(t_lexing **node, t_gen *gen)
{
	char	*before;
	char	*tmp;
	char	*after;
	int		p;
	int		e;

	p = find_char_pos((*node)->value, "$", 0);
	quote_checker("\0", 0);
	if (p > 1 && quote_checker((*node)->value, p - 1) == 2)
		before = ft_substr((*node)->value, 1, p - 2);
	else
		before = ft_substr((*node)->value, 1, p - 1);
	if (!before)
		before = ft_strdup("");
	tmp = expand_env_var(gen->my_env, (*node)->value);
	clean_quotes(node);
	if (!tmp)
		tmp = ft_strdup("");
	e = len_var((*node)->value, p);
	after = ft_substr((*node)->value, p + e + 1, ft_strlen((*node)->value));
	free((*node)->value);
	(*node)->value = construct_env_var(before, after, tmp);
	if (!(*node)->value)
		return (util_free(before, tmp, after));
	util_free(before, tmp, after);
}
