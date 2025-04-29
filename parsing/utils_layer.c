/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_layer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:44:49 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/29 12:04:58 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checks_layer(t_lexing *tmp, t_lexing *succ, t_gen *gen, t_lexing *lst)
{
	if (!ft_strncmp(tmp->type, "command", 8)
		&& !ft_strncmp(succ->type, "open_parenthesis", 17))
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	if (!ft_strncmp(tmp->type, "close_parenthesis", 18)
		&& !check_not_opened(tmp, lst))
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	if (!ft_strncmp(succ->type, "close_parenthesis", 18)
		&& !check_close(tmp, succ))
		return (error_exit(gen, "minishell: syntax error", 2), 0);
	return (1);
}
