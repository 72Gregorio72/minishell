/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:31:13 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/15 10:34:39 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*flag_quotes(int *in_single, int *in_double, char *value, int *j)
{
	int		i;
	char	*new_value;

	i = 0;
	new_value = (char *)malloc(ft_strlen(value) + 1);
	while (value[i])
	{
		if (value[i] == '\'' && !*in_double)
		{
			*in_single = !*in_single;
			i++;
		}
		else if (value[i] == '"' && !*in_single)
		{
			*in_double = !*in_double;
			i++;
		}
		else
			new_value[(*j)++] = value[i++];
	}
	return (new_value);
}

void	clean_quotes(t_lexing **node, t_gen *gen)
{
	char	*value;
	char	*new_value;
	int		in_single;
	int		in_double;
	int		j;

	value = (*node)->value;
	in_single = 0;
	in_double = 0;
	j = 0;
	new_value = flag_quotes(&in_single, &in_double, value, &j);
	if (!new_value)
		return (safe_free(gen), exit(gen->exit_status));
	new_value[j] = '\0';
	free((*node)->value);
	(*node)->value = new_value;
}

int	single_quotes(int *i, t_lexing **node)
{
	(*i)++;
	while ((*node)->value[*i] && (*node)->value[*i] != '\'')
		(*i)++;
	if ((*node)->value[*i])
		(*i)++;
	return (1);
}

void	handle_quotes(t_lexing **node, t_gen *gen)
{
	int		i;

	i = 0;
	if (!(*node)->env_variable
		&& ft_strchr((*node)->value, '=') == NULL)
		return (clean_quotes(node, gen));
	while ((*node)->value && (*node)->value[i])
	{
		if ((*node)->value[i] == '\'')
		{
			if (single_quotes(&i, node))
				continue ;
		}
		if ((*node)->value[i] == '\"')
		{
			if (double_quotes(&i, node, gen))
				continue ;
		}
		if ((*node)->value[i] == '$')
			handle_env_variable(node, gen, &i);
		i++;
	}
	if (ft_strchr((*node)->value, '=') == NULL)
		clean_quotes(node, gen);
}

int	quote_handler(t_gen *gen, t_lexing *lexed)
{
	t_lexing	*tmp;

	tmp = lexed;
	while (tmp)
	{
		if (!unclosed_quotes(tmp->value) || unclosed_quotes(tmp->value) == 2)
			return (error_exit(gen,
					"minishell: syntax error near a quote", 2), 0);
		else
		{
			if (!ft_strncmp(tmp->type, "here_doc_delimiter", 19))
				clean_quotes(&tmp, gen);
			else
				handle_quotes(&tmp, gen);
		}
		tmp = tmp->next;
	}
	return (1);
}
