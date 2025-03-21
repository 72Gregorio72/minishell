/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:34:44 by gpicchio          #+#    #+#             */
/*   Updated: 2025/03/21 15:17:46 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_command(t_gen *gen, t_lexing *node)
{
	printf("executing %s\n", node->value);
	execve(node->value, gen->av, gen->my_env);//dsadsadsa
	if (!gen->root)
		return ;
}