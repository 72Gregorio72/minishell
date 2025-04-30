/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 09:44:58 by gpicchio          #+#    #+#             */
/*   Updated: 2025/04/30 12:44:37 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	open_temp_file(int *fd, int here_doc_num)
{
	char *filename;
	char *filenum;

	filenum = ft_itoa(here_doc_num);
	filename = ft_strjoin(".here_doc_tmp", filenum);
	*fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	free(filename);
	free(filenum);
	if (*fd < 0)
	{
		printf(RED"Error opening temporary file: %s"RESET"\n",
			strerror(errno));
		exit(1);
	}
}

int	ft_pipex(int ac, char **av, char **envp)
{
	t_data_bonus	data;
	int				offset;

	if (ac < 5 || (ft_strncmp(av[1], "here_doc", 8) == 0 && ac < 6))
		return (printf(RED "Invalid number of arguments." RESET "\n"), 0);
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
		offset = 1;
	else
		offset = 0;
	initialize_data(&data, ac - offset, av + offset, envp);
	open_files(ac, av, &data);
	parse_commands(&data);
	pipex(&data);
	if (close(data.fd1) < 0 || close(data.fd2) < 0)
		return (printf(strerror(errno), 0));
	exit_free(&data);
	if (offset)
		unlink(".here_doc_tmp");
	return (0);
}
