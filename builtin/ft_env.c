/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:00:02 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/07 09:09:46 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(char *str, int fd)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		if (str[i] == '=' && !found)
		{
			write(fd, "\"", 1);
			found = 1;
		}
		i++;
	}
}

int	ft_env(char **env, int export, t_lexing *node)
{
	int	i;

	i = 0;
	if (export)
	{
		while (env[i])
		{
			write(node->outfile, "declare -x ", 11);
			print_env(env[i], node->outfile);
			write(node->outfile, "\"\n", 2);
			i++;
		}
		return (1);
	}
	else
	{
		while (env[i])
		{
			ft_putendl_fd(env[i], node->outfile);
			i++;
		}
	}
	return (1);
}
