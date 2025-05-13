/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 09:46:02 by gpicchio          #+#    #+#             */
/*   Updated: 2025/04/08 12:49:04 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include "structures_bonus.h"
# include "minishell.h"

# define RED "\033[1;31m"
# define RESET "\033[0m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"

typedef struct s_gen	t_gen;

char	*find_path_variable(char **envp);
char	**split_paths(char *path);
char	**get_paths(char **envp);
char	*get_path(char *cmd, char **envp);
char	*ft_strtok(char *str, const char *delim);
void	son_process(t_data_bonus *data, int index, pid_t *pids);
void	do_forks(t_data_bonus *data);
void	pipex(t_data_bonus *data);
int		empty_str(char *s);
void	exit_free(t_data_bonus *data);
void	open_temp_file_for_reading(int *fd, int *here_doc_num);
void	handle_here_doc(char *limiter, t_lexing *node,
			int *here_doc_num, t_gen *gen);
void	open_files(int ac, char **av, t_data_bonus *data);
void	parse_commands(t_data_bonus *data);
void	write_to_temp_file(int fd, char *limiter, t_gen *gen);
void	create_pipes(t_data_bonus *data);
void	wait_for_children(t_data_bonus *data, pid_t *pids);
void	fork_processes(t_data_bonus *data, pid_t *pids);
void	do_forks(t_data_bonus *data);
void	pipex(t_data_bonus *data);
char	*search_paths(char *cmd, char **paths);
int		get_cmd_num(int ac);
void	initialize_data(t_data_bonus *data, int ac, char **av, char **envp);
void	open_temp_file(int *fd, int here_doc_num);
void	close_all_pipes(t_data_bonus *data);
int		ft_pipex(int ac, char **av, char **envp);
char	*expand(char *str, t_gen *gen);
void	open_redirections(t_lexing *node, t_gen *gen);
void	handler_here(int sig);
void	util_signal(void);

#endif