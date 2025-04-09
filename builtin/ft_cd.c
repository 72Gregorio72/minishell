<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:52:22 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/08 13:58:27 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*construct_new_val(char *origin, int equal_pos, char *path)
{
	char	*before;

	before = ft_substr(origin, 0, equal_pos + 1);
	if (!before)
		return (NULL);
	free(origin);
	origin = ft_strjoin(before, path);
	if (!origin)
		return (free(before), NULL);
	free(before);
	return (origin);
}

int	ft_cd(char *new_path, char **export_env, t_gen *gen)
{
	int		res;
	char	*path;
	int		i;
	int		equal_pos;

	path = getcwd(NULL, 0);
	i = 0;
	while (export_env[i])
	{
		if (!ft_strncmp("OLDPWD", export_env[i], 6))
		{
			equal_pos = find_char_pos(export_env[i], "=", 0);
			if (equal_pos != -1)
				export_env[i] = construct_new_val(export_env[i],
						equal_pos, path);
		}
		i++;
	}
	res = chdir(new_path);
	if (res == -1)
	{
		error_exit(gen, "minishell: cd error", 1);
		return (free(path), 0);
	}
	return (free(path), 1);
}
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:52:22 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/08 16:39:39 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*construct_new_val(char *origin, int equal_pos, char *path)
{
	char	*before;

	before = ft_substr(origin, 0, equal_pos + 1);
	if (!before)
		return (NULL);
	free(origin);
	origin = ft_strjoin(before, path);
	if (!origin)
		return (free(before), NULL);
	free(before);
	return (origin);
}

void	update_oldpwd(char **export_env, char *path, t_gen *gen, int i)
{
	int	equal_pos;

	while (export_env[i])
	{
		if (!ft_strncmp("OLDPWD", export_env[i], 6))
		{
			equal_pos = find_char_pos(export_env[i], "=", 0);
			if (equal_pos != -1)
				export_env[i] = construct_new_val(export_env[i],
						equal_pos, path);
		}
		i++;
	}
	i = 0;
	while (gen->my_env[i])
	{
		if (!ft_strncmp("OLDPWD", gen->my_env[i], 6))
		{
			equal_pos = find_char_pos(gen->my_env[i], "=", 0);
			if (equal_pos != -1)
				gen->my_env[i] = construct_new_val(gen->my_env[i],
						equal_pos, path);
		}
		i++;
	}
}

void	update_pwd(char **export_env, char *path, t_gen *gen, int i)
{
	int	equal_pos;

	while (export_env[i])
	{
		if (!ft_strncmp("PWD", export_env[i], 3))
		{
			equal_pos = find_char_pos(export_env[i], "=", 0);
			if (equal_pos != -1)
				export_env[i] = construct_new_val(export_env[i],
						equal_pos, path);
		}
		i++;
	}
	i = 0;
	while (gen->my_env[i])
	{
		if (!ft_strncmp("PWD", gen->my_env[i], 3))
		{
			equal_pos = find_char_pos(gen->my_env[i], "=", 0);
			if (equal_pos != -1)
				gen->my_env[i] = construct_new_val(gen->my_env[i],
						equal_pos, path);
		}
		i++;
	}
}

int	ft_cd(char *new_path, char **export_env, t_gen *gen)
{
	int		res;
	char	*path;
	int		i;

	path = getcwd(NULL, 0);
	i = 0;
	update_oldpwd(export_env, path, gen, i);
	res = chdir(new_path);
	if (res == -1)
	{
		error_exit(gen, "minishell: cd error", 1);
		return (free(path), 0);
	}
	free(path);
	path = getcwd(NULL, 0);
	i = 0;
	update_pwd(export_env, path, gen, i);
	return (free(path), 1);
}
>>>>>>> origin/vcastald
