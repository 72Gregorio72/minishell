/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 10:10:31 by vcastald          #+#    #+#             */
/*   Updated: 2024/12/16 09:33:05 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memset(void *s, int c, size_t size)
{
	unsigned char			*str;
	size_t					i;

	i = 0;
	str = (unsigned char *)s;
	while (i < size)
	{
		str[i] = c;
		i++;
	}
	return (s);
}
