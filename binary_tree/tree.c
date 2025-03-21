/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:13:12 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/21 12:14:45 by gpicchio         ###   ########.fr       */
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

	if (!lexed || lexed == end)
		return (NULL);
	max = find_max_strength(lexed, lexed, end);
	if (!max)
		return (NULL);
	left = fill_tree(lexed, max, tree);
	right = fill_tree(max->next, end, tree);
	tree = new_node(max, left, right);
	return (tree);
}
// las || (echo ciao && (cat in | wc))
