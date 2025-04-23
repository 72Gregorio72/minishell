/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:03:21 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/18 13:24:43 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* int	i;

i = 0;
while (env[i])
{
	if (ft_strncmp("PWD", env[i], 3) == 0)
	{
		while (*env[i] != '=')
			(env[i]++);
		env[i]++;
		ft_putendl_fd(env[i], 1);
	}
	i++;
} */

int	ft_pwd(char **env, int fd)
{
	char	*path;

	(void)env;
	path = getcwd(NULL, 0);
	if (!path)
		return (0);
	ft_putendl_fd(path, fd);
	free(path);
	return (1);
}
