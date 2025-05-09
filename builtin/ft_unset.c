/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:37:47 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/09 11:31:26 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	checks_unset_export(const char *str)
{
	int	i;

	if (ft_isdigit(str[0]))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (str[i] != '_' && !ft_isalnum(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_unset_export(char ***env, const char *var)
{
	int	i;
	int	j;
	int	equal_pos;
	int	var_pos;

	if (!var)
		return ;
	i = -1;
	while ((*env)[++i] != NULL)
	{
		equal_pos = find_char_pos((*env)[i], "=", 0);
		var_pos = find_char_pos((char *)var, "=", 0);
		if (ft_strncmp((*env)[i], var, var_pos) == 0
			|| ft_strncmp((*env)[i], var, equal_pos) == 0)
		{
			free((*env)[i]);
			j = i;
			while ((*env)[j] != NULL)
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			return ;
		}
	}
}

void	ft_unset(char ***env, const char *var)
{
	int	i;
	int	j;

	if (!var)
		return ;
	i = 0;
	while ((*env)[i] != NULL)
	{
		if (ft_strncmp((*env)[i], var, ft_strlen(var)) == 0 &&
			(*env)[i][ft_strlen(var)] == '=')
		{
			free((*env)[i]);
			j = i;
			while ((*env)[j] != NULL)
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

int	call_unset(char **command, t_gen *gen)
{
	int	i;

	i = 1;
	while (command[i])
	{
		if (!checks_unset_export(command[i]))
		{
			ft_putstr_fd(RED"minishell: unset: ", 2);
			ft_putstr_fd(YELLOW"\'", 2);
			ft_putstr_fd((char *)command[i], 2);
			ft_putstr_fd("\'", 2);
			ft_putstr_fd(RED": not a valid identifier"RESET, 2);
			error_exit(gen, "", 1);
			i++;
			continue ;
		}
		ft_unset(&gen->my_env, command[i]);
		ft_unset_export(&gen->export_env, command[i]);
		i++;
	}
	if (gen->exit_status != 0)
		return (0);
	return (1);
}

int	call_export(t_gen *gen, t_lexing *node)
{
	int	i;

	i = 1;
	while (node->command[i])
	{
		if (!ft_export(node->command[i], gen, node, &gen->export_env))
			return (0);
		i++;
	}
	if (gen->exit_status != 0)
		return (0);
	return (1);
}
