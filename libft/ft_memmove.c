/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:22:09 by vcastald          #+#    #+#             */
/*   Updated: 2024/11/18 16:23:37 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static void	*ft_rev(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dest + (n - 1);
	s = (unsigned char *)src + (n - 1);
	while (n--)
	{
		*d = *s;
		d--;
		s--;
	}
	return (dest);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	if (!dest && !src)
		return (NULL);
	if (dest == src || n == 0)
		return (dest);
	if (src > dest)
		return (ft_memcpy(dest, src, n));
	else
		return (ft_rev(dest, src, n));
}

/*int main()
{
	char *str  = malloc(10);
	char *str2 = malloc(10);

	strcpy(str, "ciao sono io");
	strcpy(str2, "123");
	memmove(str + 5, str, 8);
	ft_memmove(str2 + 2, str2, 2);
	printf("%s\n", str);
	printf("%s\n", str2);
	return (0);
}*/
