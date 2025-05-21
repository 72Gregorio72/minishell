/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:33:28 by vcastald          #+#    #+#             */
/*   Updated: 2025/05/21 12:54:25 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	util_str(char *before, char *after, char *tmp, char **str)
{
	*str = construct_env_var(before, after, tmp);
	util_free_env_var(before, tmp, after);
}

char	*expand(char *str, t_gen *gen)
{
	int		i;
	char	*before;
	char	*tmp;
	char	*after;
	int		len;

	i = -1;
	len = 0;
	str = ft_strjoin(str, "\n");
	while (str[++i])
	{
		if (str[i] == '$')
		{
			before = ft_substr(str, 0, i);
			if (!before)
				return (NULL);
			tmp = build_tmp(gen, &len, &str, i);
			after = ft_substr(str, i + len + 1, ft_strlen(str));
			if (!after)
				return (free(before), free(tmp), NULL);
			free(str);
			util_str(before, after, tmp, &str);
		}
	}
	return (str);
}

void	open_redirections(t_lexing *node, t_gen *gen)
{
	t_lexing	*tmp;

	tmp = node;
	while (tmp)
	{
		if (!ft_strncmp(tmp->type, "outfile", 8))
			util_outfile(tmp->value, gen, tmp, 1);
		else if (!ft_strncmp(tmp->type, "infile", 7))
			util_infile(tmp->value, gen, tmp, tmp->value);
		tmp = tmp->next;
	}
}

void	handler_here(int sig)
{
	if (sig == SIGQUIT)
		write(0, "\b\b  \b\b", 6);
	else if (sig == SIGINT)
	{
		rl_done = 1;
		close(0);
	}
}

void	util_signal(void)
{
	signal(SIGQUIT, handler_here);
	signal(SIGINT, handler_here);
}
