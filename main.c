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

void	init(t_gen *gen)
{
	if (gen->lexed_data != NULL)
	{
		if (check_files(gen) && find_files(gen->lexed_data, gen))
		{
			find_args(gen->lexed_data);
			if (layerize(gen))
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
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (perror("Sigaction error"));
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		line = readline("minishell$ ");
		if (checks(line, gen))
			continue ;
		add_history(line);
		if (sigaction(SIGQUIT, &sa, NULL) == -1)
			return (perror("Sigaction error"));
		gen->av = ft_split_quote(line, ' ');
		if (!gen->av)
		{
			safe_free(gen);
			exit(0);
		}
		gen->lexed_data = lexer(gen->av, gen);
		init(gen);
		util_free(gen, line);
	}
}

//gpicchio king
int	main(int ac, char **av, char **env)
{
	struct sigaction	sa;
	t_gen				gen;

	(void)av;
	gen.my_env = copy_matrix(env);
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

/*
0. Layering e sysntax error per parentesi (D )
1. Espandere quotes --> blocca su sysntax error (D)
2. Redirections 
(mettere le flag di input ed output file) --> blocca su file_input_not found
3. Copia della list e clean_list 
(togliere parentesi, redirections, arguments e options) (D)
4. Costruisci tree sulla lista pulita (D)
5. Esegui dal tree 

*/