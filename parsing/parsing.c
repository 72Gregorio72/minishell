/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:15:26 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 09:58:24 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_command(t_lexing *node)
{
	char		**command;
	t_lexing	*tmp;
	int			i;

	i = 0;
	tmp = node;
	printf("tmp->type = %s\n", tmp->type);
	if (ft_strncmp(tmp->type, "command", 9)
		&& ft_strncmp(tmp->type, "here_doc", 9)
		&& ft_strncmp(tmp->type, "here_doc_delimiter", 19))
		{
			printf("Error %s: not a command\n", tmp->type);
			return (NULL);
		}
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
			char **command = get_command(tmp);
			if (!command)
				return (NULL);
			t_lexing *new_node = ft_lstnew_cleaned(ft_strdup(tmp->value),
					ft_strdup(tmp->type), tmp->strength, command);
			new_node->outfile = tmp->outfile;
			new_node->infile = tmp->infile;
			if (!new_node)
				return (NULL);
			ft_lstadd_back(&head, new_node);
		}
		tmp = tmp->next;
	}
	return (head);
}

// int	setup_redirections(t_lexing *node)
// {
// 	if (node->infile != -1 && node->infile < 1000)
// 	{
// 		int fd = open((char *)node->infile, O_RDONLY);
// 		if (fd == -1)
// 		{
// 			perror("infile open failed");
// 			return (0);
// 		}
// 		node->infile = fd;
// 	}
// 	if (node->outfile != -1 && node->outfile < 1000)
// 	{
// 		int fd = open((char *)node->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 		if (fd == -1)
// 		{
// 			perror("outfile open failed");
// 			if (node->infile > 2)
// 				close(node->infile);
// 			return (0);
// 		}
// 		node->outfile = fd;
// 	}
// 	return (1);
// }

int	parsing(t_gen *gen)
{
	t_lexing	*tmp;
	t_lexing	*tmp2;

	gen->root = NULL;
	if (!quote_handler(gen) || !find_red(gen->lexed_data, gen)
		|| !check_here_doc(gen) || !check_wildcards(gen)
		|| !check_operators(gen))
		return (0);
	if (!ft_strncmp(gen->lexed_data->value, "poke", 4))
		ft_pokemon();
	gen->cleaned_data = clean_data(gen);
	if (!gen->cleaned_data)
	{
		ft_putstr_fd("Error: failed to clean data\n", 2);
		return (0);
	}
	//print_list(gen->cleaned_data);
	if (ft_lstsize(gen->cleaned_data) != 2)
		gen->root = fill_tree(gen->cleaned_data,
				ft_lstlast(gen->cleaned_data), gen->root);
	//print_binary_tree(gen->root, 0);
	tmp = gen->cleaned_data;
	tmp2 = gen->lexed_data;
	if (find_cmd_num(tmp2) > 1)
		exec_command(gen);
	else
	{
		exec_single_command(gen, tmp);
	}
	ft_treeclear(gen->root);
	return (1);
}
