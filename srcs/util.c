/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:10:57 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/16 14:41:51 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
	return (1);
}

void	sort_export(t_gen *gen)
{
	int		i;
	int		j;

	i = 0;
	while (gen->export_env[i])
	{
		j = i + 1;
		while (gen->export_env[j])
		{
			if (ft_strncmp(gen->export_env[i], gen->export_env[j],
					ft_strlen(gen->export_env[i])) > 0)
				ft_swap(&gen->export_env[i], &gen->export_env[j]);
			j++;
		}
		i++;
	}
}

char	**copy_matrix(char **src)
{
	int		i;
	int		count;
	char	**dest;

	i = 0;
	count = 0;
	while (src[i])
	{
		count++;
		i++;
	}
	dest = (char **)malloc((count + 1) * sizeof(char *));
	if (!dest)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dest[i] = ft_strdup(src[i]);
		if (!dest[i])
			return (NULL);
		i++;
	}
	dest[i] = NULL;
	return (dest);
}

void	print_list(t_lexing *lst)
{
	t_lexing	*tmp;

	tmp = lst;
	while (tmp)
	{
		printf("value: %s\n", tmp->value);
		printf("outfile: %d\n", tmp->outfile);
		printf("infile: %d\n", tmp->infile);
		if (tmp->command)
		{
			int k = 0;
			while (tmp->command[k])
			{
				printf("command[%d]: %s\n", k, tmp->command[k]);
				k++;
			}
		}
		printf("--------------------\n");
		tmp = tmp->next;
	}
}

void	change_shlvl(char ***env)
{
	int		i;
	char	*equal;
	int		res;
	char	*new_value;
	char	*new_var;

	i = -1;
	while ((*env)[++i])
	{
		if (!ft_strncmp("SHLVL=", (*env)[i], 6))
		{
			equal = ft_strchr((*env)[i], '=');
			if (equal && *(equal + 1))
				res = ft_atoi(equal + 1);
			else
				res = 0;
			res++;
			new_value = ft_itoa(res);
			new_var = ft_strjoin("SHLVL=", new_value);
			free((*env)[i]);
			(*env)[i] = new_var;
			free(new_value);
			break ;
		}
	}
}
