/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_lexing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:19:27 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/19 12:09:57 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	other_checks(int *i, t_lexing **lexed, char *word)
{
	if (ft_strncmp(&word[*i], "(", ft_strlen("(")) == 0)
	{
		add_token(lexed, "(", "open_parenthesis", -1);
		*i += ft_strlen("(");
	}
	else if (ft_strncmp(&word[*i], ")", ft_strlen(")")) == 0)
	{
		add_token(lexed, ")", "close_parenthesis", -1);
		*i += ft_strlen(")");
	}
	else if (ft_strncmp(&word[*i], "\'", ft_strlen("\'")) == 0)
	{
		add_token(lexed, "\'", "single_quote", -1);
		*i += ft_strlen("\'");
	}
	else if (ft_strncmp(&word[*i], "\"", ft_strlen("\"")) == 0)
	{
		add_token(lexed, "\"", "double_quote", -1);
		*i += ft_strlen("\"");
	}
	else if (ft_strncmp(&word[*i], "||", ft_strlen("||")) == 0)
	{
		add_token(lexed, "||", "or_operator", 3);
		*i += ft_strlen("||");
	}
}

void	other_checks_1(int *i, t_lexing **lexed, char *word)
{
	if (ft_strncmp(&word[*i], "&&", ft_strlen("&&")) == 0)
	{
		add_token(lexed, "&&", "and_operator", 3);
		*i += ft_strlen("&&");
	}
	else if (ft_strncmp(&word[*i], "<<", ft_strlen("<<")) == 0)
	{
		add_token(lexed, "<<", "here_doc", 1);
		*i += ft_strlen("<<");
	}
	else if (ft_strncmp(&word[*i], ">>", ft_strlen(">>")) == 0)
	{
		add_token(lexed, ">>", "output_append", 1);
		*i += ft_strlen(">>");
	}
	else if (ft_strncmp(&word[*i], "<", ft_strlen("<")) == 0)
	{
		add_token(lexed, "<", "redirect_input", 1);
		*i += ft_strlen("<");
	}
	else if (ft_strncmp(&word[*i], ">", ft_strlen(">")) == 0)
	{
		add_token(lexed, ">", "redirect_output", 1);
		*i += ft_strlen(">");
	}
}

void	check_pipe(int *i, t_lexing **lexed, char *word)
{
	if (ft_strncmp(&word[*i], "|", ft_strlen("|")) == 0)
	{
		add_token(lexed, "|", "pipe", 2);
		*i += ft_strlen("|");
	}
}
