/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 11:10:57 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/06 14:23:53 by gpicchio         ###   ########.fr       */
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
		if (tmp->prev)
			printf("prev: %s\n", tmp->prev->value);
		printf("--------------------\n");
		tmp = tmp->next;
	}
}
