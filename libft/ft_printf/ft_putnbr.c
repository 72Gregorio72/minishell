/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:34:20 by vcastald          #+#    #+#             */
/*   Updated: 2024/12/02 16:21:06 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf.h"

int	ft_putnbr(int nb)
{
	char		fin;
	int			count;

	count = 0;
	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	else
	{
		if (nb < 0)
		{
			write(1, "-", 1);
			nb = -nb;
			count++;
		}
		if (nb > 9)
			count += ft_putnbr(nb / 10);
		fin = (nb % 10) + '0';
		count += write(1, &fin, 1);
	}
	return (count);
}

int	ft_putnbr_u(unsigned int nb)
{
	char	fin;
	int		count;

	count = 0;
	if (nb > 9)
		count += ft_putnbr_u(nb / 10);
	fin = (nb % 10) + '0';
	count += write(1, &fin, 1);
	return (count);
}
