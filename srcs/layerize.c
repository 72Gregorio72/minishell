/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layerize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 10:12:41 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/18 11:11:48 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	layerize(t_gen *gen)
{
	int			i;
	t_lexing	*tmp;

	i = 0;
	tmp = gen->lexed_data;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, "open_parenthesis", 17) == 0)
			i++;
		else if (ft_strncmp(tmp->type, "close_parenthesis", 18) == 0)
			i--;
		tmp->layer = i;
		tmp = tmp->next;
	}
}

// las || (echo ciao && (cat in | wc))