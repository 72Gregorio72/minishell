/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:49:05 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/23 11:49:37 by vcastald         ###   ########.fr       */
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

void	add_in_env(char ***env, const char *var, int append)
{
	int		i;
	int		equal_pos;

	i = -1;
	while ((*env)[++i] != NULL)
	{
		equal_pos = env_var_len((*env)[i]);
		if (ft_strncmp((*env)[i], var, equal_pos) == 0
			&& (int)env_var_len((*env)[i]) == env_var_len((char *)var))
		{
			if (append)
				ft_join(env, i, var, equal_pos);
			else
			{
				free((*env)[i]);
				(*env)[i] = ft_strdup(var);
			}
			return ;
		}
	}
	util(i, var, env);
}

void	add_in_export_env(char ***env, const char *var, int flag, int append)
{
	int		i;
	int		e_pos;
	int		var_pos;

	i = -1;
	while ((*env)[++i] != NULL)
	{
		e_pos = env_var_len((*env)[i]);
		var_pos = find_char_pos((char *)var, "=", 0);
		if (ft_strncmp((*env)[i], var, e_pos) == 0
			&& (int)env_var_len((*env)[i]) == env_var_len((char *)var))
		{
			if (flag)
				norm_add_exp_env(env, i, var, append);
			else if (append)
			{
				ft_join(env, i, "=", -1);
				ft_join(env, i, var, var_pos);
			}
			return ;
		}
	}
	util(i, var, env);
}

int	ft_export(const char *var, t_gen *gen, t_lexing *node, char ***export_env)
{
	int	equal_pos;
	int	append;

	if (!var)
		return (ft_env(*export_env, 1, node), 1);
	if (!checks_unset_export(var, 1))
	{
		ft_putstr_fd(RED"minishell: export: ", 2);
		ft_putstr_fd(YELLOW"\'", 2);
		ft_putstr_fd((char *)var, 2);
		ft_putstr_fd("\'", 2);
		ft_putstr_fd(RED": not a valid identifier"RESET, 2);
		return (error_exit(gen, "", 1), 2);
	}
	equal_pos = find_char_pos((char *)var, "=", 0);
	append = 0;
	if (equal_pos != -1 && var && var[equal_pos]
		&& equal_pos > 0 && var[equal_pos - 1] == '+')
	{
		convert_plus_equal_to_equal((char *)var);
		append = 1;
	}
	do_export(gen, equal_pos, append, var);
	return (1);
}
