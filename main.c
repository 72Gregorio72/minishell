/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 10:42:45 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/21 16:29:23 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_list(t_lexing *lst)
{
	while (lst)
	{
		printf("%s                          %.10s                                layer: %d         env: %d\n", lst->value, lst->type, lst->layer, lst->env_variable);
		lst = lst->next;
	}
}

void	loop(int ac, t_gen *gen, struct sigaction sa)
{
	char				*line;

	(void)ac;
	if (!set_signals(sa))
		return (perror("Sigaction error"));
	while (1)
	{
		line = readline("minishell$ ");
		if (checks(line, gen))
			continue ;
		add_history(line);
		gen->av = ft_split(line, ' ');
		if (!gen->av)
		{
			safe_free(gen);
			exit(0);
		}
		gen->lexed_data = lexer(gen->av);
		layerize(gen);
		print_list(gen->lexed_data);
		parsing(gen);
		free_matrix(gen->av);
		ft_lstclear(gen->lexed_data);
		free(line);
	}
}

//vcastald king
int	main(int ac, char **av, char **env)
{
	struct sigaction	sa;
	t_gen				gen;

	(void)av;
	gen.my_env = copy_matrix(env);
	gen.exit_status = 0;
	if (!gen.my_env)
		exit(1);
	sa.sa_sigaction = handler;
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	loop(ac, &gen, sa);
	safe_free(&gen);
	return (0);
}
