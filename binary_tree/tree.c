/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:13:12 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/21 09:44:19 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexing	*find_max_strength(t_lexing *lexed, t_lexing *max, t_lexing *last)
{
	t_lexing	*tmp;

	tmp = lexed;
	if (tmp == last)
		return (NULL);
	if (tmp->layer <= max->layer && tmp->strength > max->strength)
		max = tmp;
	if (tmp->next == last)
		return (max);
	return (find_max_strength(tmp->next, max, last));
}

void	print_binary_tree(t_tree *node, int depth)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	print_binary_tree(node->right, depth + 1);
	while (i < depth)
	{
		printf("    ");
		i++;
	}
	printf("🌳 %s\n", node->data->value);
	print_binary_tree(node->left, depth + 1);
}

t_tree	*fill_tree(t_lexing *lexed, t_lexing *end, t_tree *tree)
{
	t_lexing	*max;
	t_tree		*left;
	t_tree		*right;
	t_tree		*real_tree;

	max = find_max_strength(lexed, lexed, end);
	left = new_node(find_max_strength(lexed, lexed, max), NULL, NULL);
	right = new_node(find_max_strength(max->next, max->next, end), NULL, NULL);
	real_tree = new_node(max, left, right);
	left = new_node(find_max_strength(max->next, max->next, find_max_strength(right->data, right->data, end)), NULL, NULL);
	max = find_max_strength(right->data, right->data, end);
	right = new_node(find_max_strength(max->next, max->next, end), NULL, NULL);
	real_tree->right = new_node(max, left, right);
	print_binary_tree(real_tree, 10);
	return (tree);
}
