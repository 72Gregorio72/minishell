/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:03:05 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 11:11:00 by vcastald         ###   ########.fr       */
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

/* int	ft_echo(t_lexing *node, t_gen *gen, int fd, int start)
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
 	else if (succ->env_variable && succ->value[0] == '$')
	{
		tmp = expand_env_var(gen->my_env, succ->value, 0, );
		ft_putendl_fd(tmp, fd);
	} 
	else if (succ->value && ft_strncmp("-n", succ->value, ft_strlen("-n")) == 0)
		ft_putstr_fd(succ->value, fd);
	else
		ft_putendl_fd(succ->value, fd);
	free(tmp);
	return (1);
} */
