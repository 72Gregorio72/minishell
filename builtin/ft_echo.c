/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:03:05 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/19 12:35:29 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_equal_pos(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

void	print_env_var(char **env, char *var, int fd, int new_line)
{
	int		i;
	int		equal_pos;
	char	*substring;

	i = 0;
	while (env[i])
	{
		substring = ft_substr(var, 1, ft_strlen(var));
		if (!ft_strncmp(substring, env[i], ft_strlen(substring)))
		{
			free(substring);
			equal_pos = find_equal_pos(env[i]);
			substring = ft_substr(env[i], equal_pos + 1, ft_strlen(env[i]));
			if (new_line)
				ft_putendl_fd(substring, fd);
			else
				ft_putstr_fd(substring, fd);
			free(substring);
			return ;
		}
		free(substring);
		i++;
	}
}

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
	else if (succ->value && succ->env_variable)
		print_env_var(gen->my_env, succ->value, fd, 1);
	else if (succ->value && ft_strncmp("-n", succ->value, ft_strlen("-n")) == 0)
	{
		succ = succ->next;
		if (succ->env_variable)
			print_env_var(gen->my_env, succ->value, fd, 0);
		else
			ft_putstr_fd(succ->value, fd);
	}
	else
		ft_putendl_fd(succ->value, fd);
	return (1);
}
