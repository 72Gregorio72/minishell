/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:33:28 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/07 10:34:00 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*expand(char *str, t_gen *gen)
{
	int		i;
	char	*before;
	char	*tmp;
	char	*after;
	int		len;

	i = -1;
	len = 0;
	while (str[++i])
	{
		if (str[i] == '$')
		{
			before = ft_substr(str, 0, i);
			if (!before)
				return (NULL);
			tmp = build_tmp(gen, &len, &str, i);
			after = ft_substr(str, i + len + 1, ft_strlen(str));
			if (!after)
				return (free(before), free(tmp), NULL);
			free(str);
			str = construct_env_var(before, after, tmp);
			util_free_env_var(before, tmp, after);
		}
	}
	return (str);
}
