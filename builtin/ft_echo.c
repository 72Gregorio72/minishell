/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:03:05 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/24 10:14:25 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_lexing *node, t_gen *gen, int fd)
{
	t_lexing	*succ;

	if (!node->next)
		return (1);
	succ = node->next;
	if (succ->value && ft_strncmp("$?", succ->value, ft_strlen("$?")) == 0)
	{
		ft_putnbr_fd(gen->exit_status, fd);
		printf("\n");
	}
	else if (succ->value && ft_strncmp("-n", succ->value, ft_strlen("-n")) == 0)
		ft_putstr_fd(succ->value, fd);
	else
		ft_putendl_fd(succ->value, fd);
	return (1);
}
