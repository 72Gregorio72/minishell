/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:13:30 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/24 14:31:33 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_char_pos(char *s, char *chars, int start)
{
	int	i;
	int	j;

	i = start;
	while (s[i])
	{
		j = 0;
		while (chars[j])
		{
			if (s[i] == chars[j])
				return (i);
			j++;
		}
		i++;
	}
	return (0);
}

char	*expand_env_var(char **env, char *var)
{
	int		i;
	int		equal_pos;
	char	*substring;

	i = 0;
	substring = NULL;
	while (env[i])
	{
		substring = ft_substr(var, 1, ft_strlen(var));
		if (!substring)
			return (NULL);
		if (!ft_strncmp(substring, env[i], ft_strlen(substring)))
		{
			free(substring);
			equal_pos = find_char_pos(env[i], "=", 0);
			substring = ft_substr(env[i], equal_pos + 1, ft_strlen(env[i]));
			if (!substring)
				return (NULL);
			return (substring);
		}
		free(substring);
		i++;
	}
	return (substring);
}
