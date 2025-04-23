/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:47:43 by vcastald          #+#    #+#             */
/*   Updated: 2025/03/24 14:50:18 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	len_s1;
	unsigned int	len;
	char			*dest;

	len_s1 = ft_strlen(s1);
	len = len_s1 + ft_strlen(s2) + 1;
	dest = ft_calloc(len, 1);
	if (!dest)
		return (NULL);
	ft_strlcat(dest, s1, len_s1 + 1);
	ft_strlcat(dest, s2, len);
	return (dest);
}
