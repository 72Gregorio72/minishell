/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 09:59:26 by gpicchio          #+#    #+#             */
/*   Updated: 2025/04/15 15:17:17 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	open_temp_file_for_reading(int *fd)
{
	*fd = open(".here_doc_tmp", O_RDONLY);
	if (*fd < 0)
	{
		printf(RED"Error opening temporary file for reading: %s"RESET"\n",
			strerror(errno));
		exit(1);
	}
}

void	handle_here_doc(char *limiter, t_lexing *node)
{
	int	fd;

	open_temp_file(&fd);
	write_to_temp_file(fd, limiter);
	close(fd);
	open_temp_file_for_reading(&node->infile);
}

void	open_files(int ac, char **av, t_data_bonus *data)
{
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		//handle_here_doc(av[2], data);
		data->fd2 = open(av[ac - 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	}
	else
	{
		data->fd1 = open(av[1], O_RDONLY);
		if (data->fd1 < 0)
		{
			printf(RED"%s: %s"RESET"\n", strerror(errno), av[1]);
			exit(1);
		}
		data->fd2 = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	if (data->fd2 < 0)
	{
		printf(RED"%s: %s"RESET"\n", strerror(errno), av[ac - 1]);
		exit(1);
	}
}

void	parse_commands(t_data_bonus *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_num)
	{
		data->cmds[i] = ft_split(data->av[i + 2], ' ');
		if (!data->cmds[i])
		{
			printf(RED "Error: Memory allocation failed"RESET"\n");
			exit(1);
		}
		i++;
	}
}

void	write_to_temp_file(int fd, char *limiter)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd(GREEN"HEREDOC> "RESET, 1);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	get_next_line(-42);
}
