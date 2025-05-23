/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:51:48 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/02 15:39:24 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*new_node(t_lexing *lexed_input,
	t_tree *left, t_tree *right, t_tree *parent)
{
	t_tree	*d;

	d = (t_tree *)malloc(sizeof(t_tree));
	if (!d)
		return (NULL);
	d->left = left;
	d->right = right;
	d->parent = parent;
	d->data = lexed_input;
	return (d);
}

t_tree	*least_important_leaf(t_tree *tree)
{
	t_tree	*tmp;

	tmp = tree;
	while (tmp)
	{
		if (tmp->right)
			tmp = tmp->right;
		else
			break ;
	}
	return (tmp);
}

t_tree	*most_important_leaf(t_tree *tree)
{
	t_tree	*tmp;

	tmp = tree;
	while (tmp)
	{
		if (tmp->left)
			tmp = tmp->left;
		else
			break ;
	}
	return (tmp);
}

void	delone_node(t_tree *node)
{
	if (!node)
		return ;
	free(node);
}

void	ft_treeclear(t_tree *tree)
{
	if (!tree)
		return ;
	ft_treeclear(tree->left);
	ft_treeclear(tree->right);
	delone_node(tree);
}
