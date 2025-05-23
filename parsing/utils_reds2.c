/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_reds2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:53:24 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/23 15:55:55 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  check_red_node_command(t_lexing *node, int i)
{
    if (ft_strncmp(node->command[i], "<<", 2)
			&& ft_strncmp(node->command[i], "<", 1)
			&& ft_strncmp(node->command[i], ">>", 2)
			&& ft_strncmp(node->command[i], ">", 1))
        return (1);
    return (0);
}

void	remove_redirections(t_lexing *node)
{
	int		i;
	int		j;
	int		mat_length;
	char	**tmp;

	i = 0;
	j = 0;
	mat_length = calc_mat_len(node, &i);
	tmp = malloc(sizeof(char *) * (mat_length + 1));
	if (!tmp)
		return ;
	while (node->command[i])
	{
		if (check_red_node_command(node, i))
			tmp[j++] = ft_strdup(node->command[i]);
		else
			i++;
		i++;
	}
	tmp[j] = NULL;
	free_matrix(node->command);
	node->command = copy_matrix(tmp);
	free_matrix(tmp);
}
