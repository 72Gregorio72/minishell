/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:03:05 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/30 12:44:14 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_lexing *node, t_gen *gen, int fd)
{
	int			new_line;
	int			i;

	new_line = 1;
	i = 1;
	(void)gen;
	if (node->command[1] && !ft_strncmp("-n", node->command[1], 2))
	{
		new_line = 0;
		i++;
	}
	if (!node->command[1] && new_line)
		return (write(1, "\n", 1), 1);
	while (node->command[i])
	{
		if (new_line && !node->command[i + 1])
			ft_putendl_fd(node->command[i], fd);
		else
			ft_putstr_fd(node->command[i], fd);
		if (node->command[i + 1])
			write(fd, " ", 1);
		i++;
	}
	return (1);
}
