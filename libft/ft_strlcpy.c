/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:25:21 by vcastald          #+#    #+#             */
/*   Updated: 2024/11/18 16:26:44 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n - 1 && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	if (n == 0)
		return (ft_strlen(src));
	ft_strncpy(dest, src, n);
	return (ft_strlen(src));
}

/*#include <stdio.h>
int main(void)
{
	char dst[] = "ciao";
	char src[] = "mondo";
	size_t n = ft_strlcpy(dst, src, 3);
	printf("%zu", n);
	return (0);
}*/
