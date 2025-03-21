/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:49:05 by gpicchio          #+#    #+#             */
/*   Updated: 2025/03/18 15:27:24 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	**ft_realloc(char **d, size_t size)
{
	char	**new;
	size_t	i;

	new = (char **)malloc(size);
	if (!new)
		return (NULL);
	i = 0;
	while (d && d[i] != NULL)
	{
		new[i] = d[i];
		i++;
	}
	new[i] = NULL;
	free(d);
	return (new);
}

void	util(int i, const char *var, char ***env)
{
	*env = ft_realloc(*env, (i + 2) * sizeof(char *));
	if (!*env)
		return ;
	(*env)[i] = ft_strdup(var);
	(*env)[i + 1] = NULL;
}

void	ft_export(char ***env, const char *var)
{
	int		i;
	char	*equal_pos;

	if (!env || !*env)
		return ;
	if (!var)
	{
		ft_env(*env);
		return ;
	}
	equal_pos = ft_strchr(var, '=');
	if (!equal_pos)
		return ;
	i = -1;
	while ((*env)[++i] != NULL)
	{
		if (ft_strncmp((*env)[i], var, equal_pos - var) == 0 &&
			(*env)[i][equal_pos - var] == '=')
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(var);
			return ;
		}
	}
	util(i, var, env);
}
