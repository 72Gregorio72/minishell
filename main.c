/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:42:45 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/09 12:28:24 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	util_free(t_gen *gen, char *line)
{
	free_matrix(gen->av);
	ft_lstclear(gen->lexed_data, 0);
	free(line);
}

void	util_line(char **line)
{
	if (!isatty(STDIN_FILENO))
		(*line) = get_next_line(0);
	else
		(*line) = readline("minishell$ ");
}

void	init(t_gen *gen)
{
	if (gen->lexed_data != NULL)
	{
		if (check_files(gen, gen->lexed_data)
			&& find_files(gen->lexed_data, gen))
		{
			find_args(gen->lexed_data);
			if (layerize(gen, gen->lexed_data))
			{
				if (parsing(gen))
					ft_lstclear(gen->cleaned_data, 1);
			}
		}
	}
}

void	loop(int ac, t_gen *gen, struct sigaction sa)
{
	char				*line;

	(void)ac;
	while (1)
	{
		gen->fd_stdin = dup(0);
		if (isatty(STDIN_FILENO) && sigaction(SIGINT, &sa, NULL) == -1)
			return (perror("Sigaction error"));
		if (isatty(STDIN_FILENO))
			signal(SIGQUIT, SIG_IGN);
		util_line(&line);
		if (checks(line, gen))
			continue ;
		add_history(line);
		if (isatty(STDIN_FILENO) && sigaction(SIGQUIT, &sa, NULL) == -1)
			return (perror("Sigaction error"));
		gen->av = ft_split_quote(line, ' ');
		if (!gen->av)
			return (safe_free(gen), exit(gen->exit_status));
		gen->lexed_data = lexer(gen->av, gen);
		init(gen);
		dup2(gen->fd_stdin, STDIN_FILENO);
		close(gen->fd_stdin);
		util_free(gen, line);
	}
}

//vcastald king
int	main(int ac, char **av, char **env)
{
	struct sigaction	sa;
	t_gen				gen;

	(void)av;
	gen.my_env = copy_matrix(env);
	// cambiare ssh level
	gen.export_env = copy_matrix(env);
	gen.cleaned_data = NULL;
	sort_export(&gen);
	gen.exit_status = 0;
	if (!gen.my_env)
		exit(1);
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	gen.sa = sa;
	loop(ac, &gen, sa);
	safe_free(&gen);
	return (0);
}
