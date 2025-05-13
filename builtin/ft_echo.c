/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:03:05 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/09 12:10:08 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_flag(char *str)
{
	int	i;
	int	res;

	i = 1;
	if (str[0] && str[0] == '-')
		res = 1;
	else
		res = 0;
	while (str && str[0] && str[i])
	{
		if (res && str[i] != 'n')
			res = 0;
		i++;
	}
	return (res);
}

int	ft_echo(t_lexing *node, t_gen *gen, int fd)
{
	int			new_line;
	int			i;

	new_line = 1;
	i = 1;
	if (node->command[1] && check_flag(node->command[1]))
	{
		new_line = 0;
		i++;
		while (node->command[i] && check_flag(node->command[i]))
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
	return ((void)gen, 1);
}
