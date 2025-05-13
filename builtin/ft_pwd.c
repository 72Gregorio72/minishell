/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:03:21 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/09 09:15:00 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
