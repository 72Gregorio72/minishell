/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:04:54 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/23 10:46:55 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_join(char ***env, int i, const char *var, int pos)
{
	char	*tmp;

	tmp = ft_strdup((*env)[i]);
	free((*env)[i]);
	(*env)[i] = ft_strjoin(tmp, &var[pos + 1]);
	free(tmp);
}

int	find_equal(char *var, char **export_env)
{
	int		i;
	size_t	var_len;

	var_len = 0;
	while (var[var_len] && var[var_len] != '=')
		var_len++;
	i = 0;
	while (export_env[i])
	{
		if (!ft_strncmp(var, export_env[i], var_len)
			&& (export_env[i][var_len] == '\0'
			|| export_env[i][var_len] == '='))
		{
			if (find_char_pos(export_env[i], "=", 0) != -1)
				return (1);
		}
		i++;
	}
	return (0);
}

void	convert_plus_equal_to_equal(char *var)
{
	int	i;

	if (!var)
		return ;
	i = 0;
	while (var[i] && var[i + 1])
	{
		if (var[i] == '+' && var[i + 1] == '=')
		{
			while (var[i + 1])
			{
				var[i] = var[i + 1];
				i++;
			}
			var[i] = '\0';
			return ;
		}
		i++;
	}
}

void	do_export(t_gen *gen, int equal_pos, int append, const char *var)
{
	if (append && !find_equal((char *)var, gen->export_env))
	{
		add_in_export_env(&gen->export_env, var, 0, append);
		add_in_env(&gen->my_env, var, append);
	}
	else if (equal_pos == -1)
		add_in_export_env(&gen->export_env, var, 0, append);
	else
	{
		add_in_env(&gen->my_env, var, append);
		add_in_export_env(&gen->export_env, var, 1, append);
	}
}

void	norm_add_exp_env(char ***env, int i, const char *var, int append)
{
	int	var_pos;

	var_pos = find_char_pos((char *)var, "=", 0);
	if (append)
		ft_join(env, i, var, var_pos);
	else
	{
		free((*env)[i]);
		(*env)[i] = ft_strdup(var);
	}
}
