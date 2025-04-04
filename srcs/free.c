/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:56:15 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/04 09:51:31 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_matrix(char **mat)
{
	int	i;

	i = 0;
	while (mat[i])
	{
		free(mat[i]);
		i++;
	}
	free(mat);
}

void	safe_free(t_gen *gen)
{
	free_matrix(gen->av);
	ft_lstclear(gen->lexed_data);
	ft_lstclear(gen->cleaned_data);
	free_matrix(gen->my_env);
	free_matrix(gen->export_env);
	rl_clear_history();
}

void	error_exit(t_gen *gen, char *str, int exit_status)
{
	printf(RED"%s\n"RESET, str);
	rl_on_new_line();
	rl_replace_line("", 0);
	gen->exit_status = exit_status;
}
