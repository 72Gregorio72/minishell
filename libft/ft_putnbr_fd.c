/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:17:47 by vcastald          #+#    #+#             */
/*   Updated: 2024/11/21 12:17:54 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	char	fin;

	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else
	{
		if (n < 0)
		{
			write(fd, "-", 1);
			n *= -1;
		}
		if (n > 9)
			ft_putnbr_fd(n / 10, fd);
		fin = (n % 10) + '0';
		write(fd, &fin, 1);
	}
}

/*int main(void)
{
	ft_putnbr_fd(-2147483648, 1);
	return (0);
}*/
