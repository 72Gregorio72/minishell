/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:13:30 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/24 10:56:05 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_equal_pos(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			return (i);
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
			equal_pos = find_equal_pos(env[i]);
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

/*
Ordine per virgolette
- controllo se ci sono (syntax error per virgolette non chiuse)
- controllo se dollaro va espanso
- tolgo virgolette
- espando dollaro
*/