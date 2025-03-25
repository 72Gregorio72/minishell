/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:15:26 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/25 14:03:10 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_and_export(t_gen *gen, t_lexing *succ, t_lexing *node)
{
	if (ft_strncmp("export", node->value, ft_strlen("export")) == 0)
	{
		if (!succ)
			ft_env(gen->my_env);
		else
			ft_export(&gen->my_env, succ->value);
		return (1);
	}
	else if (ft_strncmp("unset", node->value, ft_strlen("unset")) == 0)
	{
		if (!succ)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
			ft_unset(&gen->my_env, succ->value);
		return (1);
	}
	return (0);
}

int	exec_builtin(t_gen *gen, t_lexing *node)
{
	t_lexing	*succ;

	succ = node->next;
	if (ft_strncmp("echo", node->value, ft_strlen("echo")) == 0)
		return (ft_echo(node, gen, 1));
	else if (ft_strncmp("env", node->value, ft_strlen("env")) == 0)
		return (ft_env(gen->my_env));
	else if (ft_strncmp("pwd", node->value, 3) == 0)
		return (ft_pwd(gen->my_env, 1));
	else if (ft_strncmp("exit", node->value, ft_strlen("exit")) == 0)
		ft_exit(gen);
	else if (ft_strncmp("cd", node->value, ft_strlen("cd")) == 0)
		return (ft_cd(succ->value));
	else
		return (unset_and_export(gen, succ, node));
	return (1);
}

// las || (echo ciao && (cat in | wc))
void	parsing(t_gen *gen)
{
	t_lexing	*tmp;
	int			flag;

	gen->root = NULL;
	flag = 0;
	if (ft_lstsize(gen->lexed_data) != 2)
	{
		gen->root = fill_tree(gen->lexed_data, ft_lstlast(gen->lexed_data), gen->root);
		print_binary_tree(gen->root, 0);
	}
	tmp = gen->lexed_data;
	if (find_cmd_num(tmp) > 1)
		exec_command(gen);
	else
		exec_single_command(gen, tmp);
	ft_treeclear(gen->root);
}
//echo -n ciao | echo -n ciao1 && echo ciao2 | ciao3