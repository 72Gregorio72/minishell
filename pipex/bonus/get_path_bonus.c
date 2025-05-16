/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 10:02:30 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/16 10:51:22 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*find_path_variable(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	**split_paths(char *path)
{
	char	**paths;
	char	*token;
	int		i;

	i = 0;
	paths = malloc(sizeof(char *) * 100);
	if (!paths)
		return (NULL);
	token = ft_strtok(path, ":");
	while (token)
	{
		paths[i++] = token;
		token = ft_strtok(NULL, ":");
	}
	free(token);
	paths[i] = NULL;
	return (paths);
}

char	**get_paths(char **envp)
{
	char	*path;

	path = find_path_variable(envp);
	if (!path)
		return (NULL);
	return (split_paths(path));
}

char	*search_paths(char *cmd, char **paths)
{
	int		i;
	char	*full_path;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free(paths);
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	*full_path;
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	paths = get_paths(envp);
	if (!paths)
		return (NULL);
	full_path = search_paths(cmd, paths);
	if (full_path)
		return (full_path);
	return (NULL);
}
