/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:36:26 by vcastald          #+#    #+#             */
/*   Updated: 2025/01/29 15:51:24 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

static int	num_len(int c)
{
	int	count;

	count = 0;
	if (c <= 0)
		count++;
	while (c != 0)
	{
		count++;
		c /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		len;
	long	l;
	char	*fin;

	len = 0;
	l = 0;
	len = num_len(n);
	l = n;
	fin = (char *)malloc((len + 1) * sizeof(char));
	if (!fin)
		return (NULL);
	fin[len] = '\0';
	while (len > 0)
	{
		fin[--len] = (l % 10) + '0';
		l = l / 10;
	}
	return (fin);
}

/*int	main(void)
{
	char *t = ft_itoa(-2147483648);
	printf("%s", t);
	return (0);
}*/
