/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/14 15:03:32 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_piped(t_tree *node)
{
	t_tree	*tmp;

	if (!node)
		return ;
	tmp = node;
	tmp->data->piped = 0;
	init_piped(node->left);
	init_piped(node->right);
}

void	exec_command(t_gen *gen)
{
	init_piped(gen->root);
	flag_piped(gen->root);
	exec_tree(gen, gen->root);
	ft_treeclear(gen->root);
	gen->root = NULL;
}
