/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:15:26 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/08 15:57:04 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_and_export(t_gen *gen, char *succ, char *curr)
{
	if (!ft_strncmp("export", curr, ft_strlen(curr)))
	{
		if (!succ)
			ft_export(&gen->my_env, NULL, &gen->export_env);
		else
			ft_export(&gen->my_env, succ, &gen->export_env);
		return (1);
	}
	else if (!ft_strncmp("unset", curr, ft_strlen(curr)))
	{
		if (!succ)
		{
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
		{
			ft_unset(&gen->my_env, succ);
			ft_unset_export(&gen->export_env, succ);
		}
		return (1);
	}
	return (0);
}

int	exec_builtin(t_gen *gen, t_lexing *node)
{
	if (ft_strncmp("echo", node->command[0], ft_strlen(node->command[0])) == 0)
		return (ft_echo(node, gen, node->outfile));
	else if (ft_strncmp("env", node->command[0],
			ft_strlen(node->command[0])) == 0)
		return (ft_env(gen->my_env, 0));
	else if (ft_strncmp("pwd", node->command[0], 3) == 0)
		return (ft_pwd(gen->my_env, node->outfile));
	else if (ft_strncmp("exit", node->command[0],
			ft_strlen(node->command[0])) == 0)
		ft_exit(gen);
	else if (ft_strncmp("cd", node->command[0],
			ft_strlen(node->command[0])) == 0
		&& node->command[1])
		return (ft_cd(node->command[1], gen->export_env, gen));
	else
		return (unset_and_export(gen, node->command[1],
				node->command[0]));
	return (1);
}

char	**get_command(t_lexing *node)
{
	char		**command;
	t_lexing	*tmp;
	int			i;

	i = 0;
	tmp = node;
	if (ft_strncmp(tmp->type, "command", 8))
		return (NULL);
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "argument", 9)
			|| !ft_strncmp(tmp->type, "option", 7)
			|| !ft_strncmp(tmp->type, "command", 8))
			i++;
		tmp = tmp->next;
	}
	command = (char **)malloc(sizeof(char *) * (i + 1));
	if (!command)
		return (NULL);
	i = 0;
	tmp = node;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "argument", 9)
			|| !ft_strncmp(tmp->type, "option", 7)
			|| !ft_strncmp(tmp->type, "command", 8))
		{
			command[i] = ft_strdup(tmp->value);
			i++;
		}
		else
			break ;
		tmp = tmp->next;
	}
	command[i] = NULL;
	return (command);
}

char	**ft_strdup_matrix(char **matrix)
{
	char	**new_matrix;
	int		i;

	i = 0;
	while (matrix[i])
		i++;
	new_matrix = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_matrix)
		return (NULL);
	i = 0;
	while (matrix[i])
	{
		new_matrix[i] = ft_strdup(matrix[i]);
		i++;
	}
	new_matrix[i] = NULL;
	return (new_matrix);
}

t_lexing	*clean_data(t_gen *gen)
{
	t_lexing	*tmp = gen->lexed_data;
	t_lexing	*head = NULL;

	while (tmp)
	{
		if (ft_strncmp(tmp->type, "argument", 9)
			&& ft_strncmp(tmp->type, "option", 7)
			&& ft_strncmp(tmp->type, "open_parenthesis", 17)
			&& ft_strncmp(tmp->type, "close_parenthesis", 18))
		{
			t_lexing *new_node = ft_lstnew_cleaned(ft_strdup(tmp->value),
					tmp->type, tmp->strength, get_command(tmp));
			if (!new_node)
				return (NULL);
			ft_lstadd_back(&head, new_node);
		}
		tmp = tmp->next;
	}
	return (head);
}

// las || (echo ciao && (cat in | wc))
int	parsing(t_gen *gen)
{
	t_lexing	*tmp;

	gen->root = NULL;
	if (!quote_handler(gen) || !find_red(gen->lexed_data, gen)
		|| !check_here_doc(gen) || !check_wildcards(gen)
		|| !check_operators(gen))
		return (0);
	if (!ft_strncmp(gen->lexed_data->value, "poke", 4))
		ft_pokemon();
	//print_list(gen->lexed_data);
	gen->cleaned_data = clean_data(gen);
	if (ft_lstsize(gen->cleaned_data) != 2)
	{
		gen->root = fill_tree(gen->cleaned_data,
				ft_lstlast(gen->cleaned_data), gen->root);
		//print_binary_tree(gen->root, 0);
	}
	tmp = gen->cleaned_data;
	if (find_cmd_num(tmp) > 1)
		exec_command(gen);
	else
		exec_single_command(gen, tmp);
	ft_treeclear(gen->root);
	return (1);
}
//echo -n ciao | echo -n ciao1 && echo ciao2 | ciao3