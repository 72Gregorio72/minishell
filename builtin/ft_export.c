/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:49:05 by gpicchio          #+#    #+#             */
/*   Updated: 2025/04/02 13:04:30 by vcastald         ###   ########.fr       */
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

void	add_in_env(char ***env, const char *var)
{
	int		i;
	int		equal_pos;

	i = -1;
	while ((*env)[++i] != NULL)
	{
		equal_pos = find_char_pos((*env)[i], "=", 0);
		if (ft_strncmp((*env)[i], var, equal_pos) == 0)
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(var);
			return ;
		}
	}
	util(i, var, env);
}

void	add_in_export_env(char ***env, const char *var, int flag)
{
	int		i;
	int		equal_pos;
	int		var_pos;

	i = -1;
	while ((*env)[++i] != NULL)
	{
		equal_pos = find_char_pos((*env)[i], "=", 0);
		var_pos = find_char_pos((char *)var, "=", 0);
		if (ft_strncmp((*env)[i], var, var_pos) == 0
			|| ft_strncmp((*env)[i], var, equal_pos) == 0)
		{
			if (flag)
			{
				free((*env)[i]);
				(*env)[i] = ft_strdup(var);
			}
			return ;
		}
	}
	util(i, var, env);
}

void	ft_export(char ***env, const char *var, char ***export_env)
{
	char	*equal_pos;

	if (!env || !*env)
		return ;
	if (!var)
	{
		ft_env(*export_env, 1);
		return ;
	}
	equal_pos = ft_strchr(var, '=');
	if (!equal_pos)
		add_in_export_env(export_env, var, 0);
	else
	{
		add_in_env(env, var);
		add_in_export_env(export_env, var, 1);
	}
}
