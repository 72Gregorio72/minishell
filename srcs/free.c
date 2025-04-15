/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:56:15 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/11 10:17:56 by vcastald         ###   ########.fr       */
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
	ft_lstclear(gen->lexed_data, 0);
	ft_lstclear(gen->cleaned_data, 1);
	ft_treeclear(gen->root);
	free_matrix(gen->my_env);
	free_matrix(gen->export_env);
	rl_clear_history();
}

void	error_exit(t_gen *gen, char *str, int exit_status)
{
	ft_putstr_fd(RED, 2);
	ft_putendl_fd(str, 2);
	ft_putstr_fd(RESET, 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	gen->exit_status = exit_status;
}

void	util_free_env_var(char *before, char *tmp, char *after)
{
	free(before);
	free(tmp);
	free(after);
}
