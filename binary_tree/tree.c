/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:13:12 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/02 16:26:00 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexing	*find_max_strength(t_lexing *lexed, t_lexing *max, t_lexing *last)
{
	t_lexing	*tmp;

	tmp = lexed;
	while (tmp && tmp != last)
	{
		if (tmp->layer <= max->layer && tmp->strength > max->strength)
			max = tmp;
		tmp = tmp->next;
	}
	return (max);
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
	i = 0;
	if (node->data->command)
	{
		printf("🌲");
		while (node->data->command[i])
		{
			printf(" %s", node->data->command[i]);
			i++;
		}
		printf("\n");
	}
	else
		printf("🌲 %s\n", node->data->value);
	print_binary_tree(node->left, depth + 1);
}

t_tree	*fill_tree(t_lexing *lexed, t_lexing *end, t_tree *parent)
{
	t_lexing	*max;
	t_tree		*left;
	t_tree		*right;
	t_tree		*node;

	if (lexed == ft_lstlast(lexed))
		return (new_node(lexed, NULL, NULL, parent));
	if (!lexed || lexed == end)
		return (NULL);
	max = find_max_strength(lexed, lexed, end);
	if (!max)
		return (NULL);
	node = new_node(max, NULL, NULL, parent);
	left = fill_tree(lexed, max, node);
	right = fill_tree(max->next, end, node);
	node->left = left;
	node->right = right;
	return (node);
}

// las || (echo ciao && (cat in | wc))
// ciao | ciao1 || ciao2 | ciao3