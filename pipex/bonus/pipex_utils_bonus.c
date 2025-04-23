/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 10:04:04 by gpicchio          #+#    #+#             */
/*   Updated: 2025/03/11 12:15:10 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*static_str;
	char		*token;
	int			i;

	if (str)
		static_str = str;
	if (!static_str)
		return (NULL);
	i = 0;
	token = static_str;
	while (static_str[i])
	{
		if (ft_strchr(delim, static_str[i]))
		{
			static_str[i] = '\0';
			static_str += i + 1;
			return (token);
		}
		i++;
	}
	static_str = NULL;
	return (token);
}

int	empty_str(char *s)
{
	int	i;

	i = -1;
	while (s[++i])
	{
		if (s[i] != ' ')
			return (0);
	}
	printf(RED"Error: %s is empty."RESET"\n", s);
	return (1);
}

int	get_cmd_num(int ac)
{
	return (ac - 3);
}

void	exit_free(t_data_bonus *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->cmd_num)
	{
		j = 0;
		while (data->cmds[i][j])
			free(data->cmds[i][j++]);
		free(data->cmds[i]);
		i++;
	}
	free(data->cmds);
}

void	initialize_data(t_data_bonus *data, int ac, char **av, char **envp)
{
	data->cmd_num = get_cmd_num(ac);
	data->envp = envp;
	data->av = av;
	data->ac = ac;
	data->cmds = malloc(sizeof(char **) * data->cmd_num);
	if (!data->cmds)
	{
		printf(RED "Memory allocation failed" RESET "\n");
		exit(1);
	}
}
