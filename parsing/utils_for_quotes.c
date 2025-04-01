/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:52:28 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/01 09:46:44 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_env_variable(t_lexing **node, t_gen *gen)
{
	char	*before;
	char	*tmp;
	char	*after;
	char	*new_value;
	int		dollar_pos;
	int		var_end;

	dollar_pos = find_char_pos((*node)->value, "$", 0);
	before = ft_substr((*node)->value, 1, dollar_pos - 1);
	if (!before)
		before = ft_strdup("");
	// clean_quotes(node);
	tmp = expand_env_var(gen->my_env, (*node)->value);
	if (!tmp)
		tmp = ft_strdup("");
	var_end = len_var((*node)->value, dollar_pos) + 1;
	after = ft_substr((*node)->value, dollar_pos + var_end, ft_strlen((*node)->value));
	new_value = (char *)malloc(ft_strlen(before) + ft_strlen(tmp) + ft_strlen(after) + 1);
	if (!new_value)
		return ;
	new_value[0] = '\0';
	ft_strlcat(new_value, before, ft_strlen(before) + 1);
	ft_strlcat(new_value, tmp, ft_strlen(new_value) + ft_strlen(tmp) + 1);
	ft_strlcat(new_value, after, ft_strlen(new_value) + ft_strlen(after) + 1);
	free((*node)->value);
	(*node)->value = new_value;
	free(before);
	free(tmp);
	free(after);
}
