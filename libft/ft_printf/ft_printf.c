/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:08:40 by vcastald          #+#    #+#             */
/*   Updated: 2024/12/16 09:33:52 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>

int	check_args(const char *str, va_list input, int pos, char *base)
{
	if (str[pos] == 'c')
		return (ft_putchar_fd_count((char)va_arg(input, int), 1));
	else if (str[pos] == 'd')
		return (ft_putnbr(va_arg(input, int)));
	else if (str[pos] == 'i')
		return (ft_putnbr(va_arg(input, int)));
	else if (str[pos] == 'u')
		return (ft_putnbr_u(va_arg(input, unsigned int)));
	else if (str[pos] == 'x')
		return (ft_put_base(va_arg(input, unsigned int), "0123456789abcdef"));
	else if (str[pos] == 'X')
		return (ft_put_base(va_arg(input, unsigned int), "0123456789ABCDEF"));
	else if (str[pos] == '%')
		return (ft_putchar_fd_count(str[pos], 1));
	else if (str[pos] == 's')
		return (ft_putstr_fd_count(va_arg(input, char *), 1));
	else if (str[pos] == 'p')
		return (ft_put_base_l(va_arg(input, unsigned long), base));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		fin;
	char	*base;

	va_start(args, str);
	i = 0;
	base = "0123456789abcdef";
	fin = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			fin += check_args(str, args, i + 1, base);
			i++;
		}
		else
		{
			write(1, &str[i], 1);
			fin++;
		}
		i++;
	}
	va_end(args);
	return (fin);
}

/*#include <stdio.h>
#include <limits.h>
int main(void)
{
	// char c = 'c';
	// printf(" %p %p ", 0, 0);
	ft_printf(" %p %p ", 0, 0);
	// printf("Norm: %d, Mio: %d\n", n, t);
	return (0);
}*/
