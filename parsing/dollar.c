/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:13:30 by vcastald          #+#    #+#             */
/*   Updated: 2025/04/02 10:30:11 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_char_pos(char *s, char *chars, int start)
{
	int	i;
	int	j;

	i = start;
	while (s[i])
	{
		j = 0;
		while (chars[j])
		{
			if (s[i] == chars[j])
				return (i);
			j++;
		}
		i++;
	}
	return (-1);
}

int	len_var(char *str, int dollar_pos)
{
	int	len;

	len = 0;
	dollar_pos++;
	while (str[dollar_pos + len]
		&& str[dollar_pos + len] != '\''
		&& (ft_isalnum(str[dollar_pos + len]) || str[dollar_pos + len] == '_'))
		len++;
	return (len);
}

char	*expand_env_var(char **env, char *var)
{
	int		i;
	int		len;
	char	*env_value;
	char	*var_name;
	int		doll_pos;

	i = -1;
	doll_pos = find_char_pos(var, "$", 0);
	len = len_var(var, doll_pos);
	var_name = ft_substr(var, doll_pos + 1, len);
	if (!var_name)
		return (NULL);
	while (env[++i])
	{
		if (!ft_strncmp(var_name, env[i], len) && env[i][len] == '=')
		{
			env_value = ft_strdup(env[i] + len + 1);
			if (!env_value)
				return (NULL);
			return (free(var_name), env_value);
		}
	}
	return (free(var_name), ft_strdup(""));
}
