/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 09:59:26 by gpicchio          #+#    #+#             */
/*   Updated: 2025/05/07 12:22:00 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
// << lim cat > b | << lim1 grep c

void	open_temp_file_for_reading(int *fd, int *here_doc_num)
{
	char	*filename;
	char	*filenum;

	filenum = ft_itoa(*here_doc_num);
	filename = ft_strjoin(".here_doc_tmp", filenum);
	*fd = open(filename, O_RDONLY);
	free(filename);
	free(filenum);
	(*here_doc_num)++;
	if (*fd < 0)
	{
		printf(RED"Error opening temporary file for reading: %s"RESET"\n",
			strerror(errno));
		exit(1);
	}
}

void	handle_here_doc(char *limiter, t_lexing *node,
		int *here_doc_num, t_gen *gen)
{
	int	fd;
	int	tmp_fd;

	open_temp_file(&fd, *here_doc_num);
	write_to_temp_file(fd, limiter, gen);
	close(fd);
	tmp_fd = 1;
	if (!node)
	{
		open_redirections(gen->lexed_data, gen);
		open_temp_file_for_reading(&tmp_fd, here_doc_num);
	}
	else
		open_temp_file_for_reading(&node->infile, here_doc_num);
}

void	open_files(int ac, char **av, t_data_bonus *data)
{
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
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

void	write_to_temp_file(int fd, char *limiter, t_gen *gen)
{
	char	*line;

	while (1)
	{
		signal(SIGQUIT, handler_here);
		signal(SIGINT, handler_here);
		if (errno == EBADF)
			break ;
		line = readline(GREEN"HEREDOC> "RESET);
		if (!line)
		{
			write(1, "\n", 1);
			break ;
		}
		line = expand(line, gen);
		if (!line)
			return (safe_free(gen), perror("malloc"), exit(gen->exit_status));
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\0')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}
