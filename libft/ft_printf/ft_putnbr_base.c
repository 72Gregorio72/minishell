/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:29:07 by vcastald          #+#    #+#             */
/*   Updated: 2024/12/02 16:57:35 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static int	ft_strlen(char *str)
{
	int	n;

	n = 0;
	while (str[n] != '\0')
	{
		n++;
	}
	return (n);
}

int	ft_put_base(unsigned int nbr, char *base)
{
	int				fin;
	unsigned int	base_len;

	fin = 0;
	base_len = ft_strlen(base);
	if (nbr >= base_len)
	{
		fin += ft_put_base(nbr / base_len, base);
		nbr %= base_len;
	}
	fin += write(1, &base[nbr], 1);
	return (fin);
}

int	ft_put_base_l(unsigned long nbr, char *base)
{
	unsigned long	base_len;
	char			buffer[64];
	int				i;
	int				count;

	if (nbr == 0)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	else
		write(1, "0x", 2);
	base_len = ft_strlen(base);
	i = 0;
	while (nbr > 0)
	{
		buffer[i++] = base[nbr % base_len];
		nbr /= base_len;
	}
	count = i + 2;
	while (--i >= 0)
		write(1, &buffer[i], 1);
	return (count);
}
