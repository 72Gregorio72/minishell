/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:00:02 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/02 11:45:59 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **env, int export)
{
	int	i;

	i = 0;
	if (export)
	{
		while (env[i])
		{
			write(1, "declare -x ", 11);
			ft_putendl_fd(env[i], 1);
			i++;
		}
		return (1);
	}
	else
	{
		while (env[i])
		{
			ft_putendl_fd(env[i], 1);
			i++;
		}
	}
	return (1);
}
