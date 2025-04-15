/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:52:28 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 13:23:29 by vcastald         ###   ########.fr       */
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

char	*construct_before(char *value, int p, int clean)
{
	char	*before;

	quote_checker("\0", 0);
	if (p > 1 && quote_checker(value, p - 1) == 2)
	{
		if (!clean)
			before = ft_substr(value, 0, p - 2);
		else
			before = ft_substr(value, 1, p - 2);
	}
	else
	{
		if (!clean)
			before = ft_substr(value, 0, p);
		else
			before = ft_substr(value, 0, p);
	}
	if (!before)
		before = ft_strdup("");
	return (before);
}

void	start_with_num(char **tmp, t_gen *gen, char *before, int *e)
{
	*tmp = ft_strdup("");
	if (!(*tmp))
	{
		perror("malloc error");
		free(before);
		safe_free(gen);
		exit(gen->exit_status);
	}
	*e = 1;
}

void	handle_env_variable(t_lexing **node, t_gen *gen, int clean)
{
	char	*before;
	char	*tmp;
	char	*after;
	int		p;
	int		e;

	p = find_char_pos((*node)->value, "$", 0);
	before = construct_before((*node)->value, p, clean);
	if (ft_isdigit((*node)->value[p + 1]))
		start_with_num(&tmp, gen, before, &e);
	else
	{
		tmp = expand_env_var(gen->my_env, (*node)->value);
		if (!tmp)
			tmp = ft_strdup("");
		e = len_var((*node)->value, p);
	}
	after = ft_substr((*node)->value, p + e + 1, ft_strlen((*node)->value));
	free((*node)->value);
	(*node)->value = construct_env_var(before, after, tmp);
	if (!(*node)->value)
		return (util_free_env_var(before, tmp, after));
	if (clean)
		clean_quotes(node, gen);
	util_free_env_var(before, tmp, after);
}

void	single_quotes(t_lexing **node, t_gen *gen)
{
	size_t	i;
	int		found;

	i = 1;
	found = 0;
	while ((*node)->value[i] && i < ft_strlen((*node)->value) - 1)
	{
		if ((*node)->value[i] == '\'')
			found = 1;
		i++;
	}
	if (found)
		handle_env_variable(node, gen, 0);
	clean_quotes(node, gen);
}
