/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:49:05 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/14 10:39:22 by vcastald         ###   ########.fr       */
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
	int		e_pos;
	int		var_pos;

	i = -1;
	while ((*env)[++i] != NULL)
	{
		e_pos = find_char_pos((*env)[i], "=", 0);
		var_pos = find_char_pos((char *)var, "=", 0);
		if (ft_strncmp((*env)[i], var, var_pos) == 0
			|| ft_strncmp((*env)[i], var, e_pos) == 0)
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

/*int	check_exists(char **env, char *var_name)
{
	int	i;
	int	equal_pos;

	i = 0;
	while (env[i])
	{
		equal_pos = find_char_pos(env[i], "=", 0);
		if (!ft_strncmp(env[i], var_name, equal_pos))
			return (1);
		i++;
	}
	return (0);
}

void	convert_plus_equal_to_equal(char *str)
{
	char	*pos;

	pos = ft_strnstr(str, "+=", ft_strlen(str));
	if (pos != NULL)
		ft_memmove(pos, pos + 1, ft_strlen(pos));
}

void	append_to_envs(char ***env, char ***export_env, char *var)
{
	int		i;
	int		equal_pos;
	char	*var_pos;
	char	*tmp;

	i = 0;
	var_pos = ft_strchr(var, '=');
	var_pos++;
	while ((*env)[i])
	{
		equal_pos = find_char_pos((*env)[i], "=", 0);
		if (ft_strncmp((*env)[i], var, equal_pos) == 0)
		{
			tmp = ft_strdup((*env)[i]);
			free((*env)[i]);
			(*env)[i] = ft_strjoin(tmp, var_pos);
			free(tmp);
			return ;
		}
		i++;
	}
	i = 0;
	while ((*export_env)[i])
	{
		equal_pos = find_char_pos((*export_env)[i], "=", 0);
		if (equal_pos == -1)
			equal_pos = ft_strlen((*export_env)[i]);
		if (ft_strncmp((*export_env)[i], var, equal_pos) == 0)
		{
			tmp = ft_strdup((*export_env)[i]);
			free((*export_env)[i]);
			(*export_env)[i] = ft_strjoin(tmp, var_pos);
			free(tmp);
			return ;
		}
		i++;
	}
} */

int	ft_export(const char *var, t_gen *gen, t_lexing *node, char ***export_env)
{
	int	equal_pos;

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
	if (equal_pos == -1)
		add_in_export_env(export_env, var, 0);
	else
	{
		add_in_env(&gen->my_env, var);
		add_in_export_env(export_env, var, 1);
	}
	return (1);
}

/* 	if (equal_pos != -1 && var[equal_pos - 1] && var[equal_pos - 1] == '+'
		&& !check_exists(gen->export_env, (char *)var)
		&& !check_exists(gen->my_env, (char *)var))
		append(&gen->export_env, &gen->my_env, (char *)var);
	else if (equal_pos != -1 && var[equal_pos - 1] && var[equal_pos - 1] == '+')
		remove_plus((char *)var); */