/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:10:57 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/09 12:50:14 by vcastald         ###   ########.fr       */
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
	while (lst)
	{
		//printf("%s                          %.10s                                layer: %d         env: %d\n", lst->value, lst->type, lst->layer, lst->env_variable);
		printf("%s: %s   Infile:%d     Outfile: %d\n", lst->type, lst->value, lst->infile, lst->outfile);	
				
/* if (lst->command)
		{
			int i = 0;
			while (lst->command[i])
			{
				printf("%s\n", lst->command[i]);
				i++;
			}
		}
		else
			printf("NULL\n");
		lst = lst->next; */

		printf("\n");
	}
}
