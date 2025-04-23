/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:45:41 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/23 14:09:26 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	rows_in_mat(char **mat)
{
	int	i;

	i = 0;
	while (mat[i])
		i++;
	return (i);
}

int	all_num(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_gen *gen, t_lexing *node)
{
	printf("exit\n");
	if (rows_in_mat(node->command) > 2)
		return (error_exit(gen, "minishell: too many arguments", 1), 0);
	if (node->command[1] && !all_num(node->command[1]))
	{
		ft_putstr_fd(RED"minishell: exit: ", 2);
		ft_putstr_fd(YELLOW, 2);
		ft_putstr_fd(node->value, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(RED"numeric argument required\n"RESET, 2);
		safe_free(gen);
		exit(2);
		return (1);
	}
	else if (node->command[1])
		gen->exit_status = ft_atoi(node->command[1]);
	if (gen->exit_status > 255)
		gen->exit_status %= 256;
	safe_free(gen);
	exit(gen->exit_status);
	return (1);
}
