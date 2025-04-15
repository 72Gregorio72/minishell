/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:37:47 by gpicchio          #+#    #+#             */
/*   Updated: 2025/04/15 12:02:32 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void	call_unset(char **command, t_gen *gen)
{
	int	i;

	i = 1;
	while (command[i])
	{
		ft_unset(&gen->my_env, command[i]);
		ft_unset_export(&gen->export_env, command[i]);
		i++;
	}
}

int	call_export(t_gen *gen, char **command)
{
	int	i;

	i = 1;
	while (command[i])
	{
		if (!ft_export(&gen->my_env, command[i], &gen->export_env, gen))
			return (0);
		i++;
	}
	return (1);
}

/* int main() {
	char **envp = malloc(4 * sizeof(char *));
	envp[0] = strdup("PATH=/usr/bin");
	envp[1] = strdup("HOME=/home/user");
	envp[2] = strdup("SHELL=/bin/bash");
	envp[3] = NULL;

	printf("### ENV iniziale ###\n");
	print_env(envp);
	printf("\n### Exporting VAR=test ###\n");
	my_export(&envp, "VAR", "test");
	print_env(envp);
	printf("\n### Updating PATH ###\n");
	my_export(&envp, "PATH", "/usr/local/bin");
	print_env(envp);
	printf("\n### Unsetting HOME ###\n");
	my_unset(&envp, "HOME");
	print_env(envp);
	for (int i = 0; envp[i] != NULL; i++) {
		free(envp[i]);
	}
	free(envp);
	return 0;
} */
