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
		return (safe_free(gen));
	new_value[j] = '\0';
	free((*node)->value);
	(*node)->value = new_value;
}

void	util_quotes(t_gen *gen, t_lexing **node, char *tmp, int bool_quote)
{
	if (bool_quote == 2)
		handle_env_variable(node, gen, 1);
	else if (bool_quote)
		single_quotes(node, gen);
	else
	{
		if (!ft_strncmp((*node)->value, "$?", 2))
		{
			free((*node)->value);
			(*node)->value = ft_itoa(gen->exit_status);
		}
		else
		{
			tmp = ft_strdup((*node)->value);
			free((*node)->value);
			if (ft_isdigit(tmp[1]))
				((*node)->value) = ft_substr(tmp, 2, ft_strlen(tmp));
			else
				(*node)->value = expand_env_var(gen->my_env, tmp);
		}
	}
}

void	handle_quotes(t_lexing **node, t_gen *gen)
{
	int		i;
	int		bool_quote;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (!(*node)->env_variable)
		clean_quotes(node, gen);
	else
	{
		quote_checker("\0", 0);
		while ((*node)->value[i])
		{
			bool_quote = quote_checker((*node)->value, i);
			if ((*node)->env_variable)
			{
				util_quotes(gen, node, tmp, bool_quote);
				break ;
			}
			i++;
		}
	}
	free(tmp);
}

int	quote_handler(t_gen *gen)
{
	t_lexing	*tmp;

	tmp = gen->lexed_data;
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

/*
QUOTE CHECKER
- NULL: ultimo valore trovato
- 1: single quote prima
- 2: doubl quote prima
- passando \0: resetta statica */

/*
Ordine per virgolette
- controllo se ci sono (syntax error per virgolette non chiuse (D))
- controllo se dollaro va espanso
- tolgo virgolette
- espando dollaro
*/