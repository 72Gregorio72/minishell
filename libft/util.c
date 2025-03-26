/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:09:53 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/26 12:45:14 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "../includes/minishell.h"

int	quote_split(char const *str, int *i, int *start, int *quote_status)
{
	while (str[*i] && str[*i] == ' ')
		(*i)++;
	*start = *i;
	*quote_status = quote_checker((char *)str, *i);
	while (str[*i] && (str[*i] != ' ' || *quote_status != 0))
		(*i)++;
	return (*i > *start);
}
