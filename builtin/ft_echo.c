/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:03:05 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/01 12:24:33 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_lexing *node, t_gen *gen, int fd)
{
	t_lexing	*succ;
	char		*tmp;

	if (!node->next)
		return (1);
	succ = node->next;
	tmp = NULL;
	if (succ->value && ft_strncmp("$?", succ->value, ft_strlen("$?")) == 0)
	{
		ft_putnbr_fd(gen->exit_status, fd);
		printf("\n");
	}
/* 	else if (succ->env_variable && succ->value[0] == '$')
	{
		tmp = expand_env_var(gen->my_env, succ->value, 0, );
		ft_putendl_fd(tmp, fd);
	} */
	else if (succ->value && ft_strncmp("-n", succ->value, ft_strlen("-n")) == 0)
		ft_putstr_fd(succ->value, fd);
	else
		ft_putendl_fd(succ->value, fd);
	free(tmp);
	return (1);
}
