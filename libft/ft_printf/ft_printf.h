/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:26:41 by vcastald          #+#    #+#             */
/*   Updated: 2024/12/16 09:30:35 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_printf(const char *str, ...);
int		ft_putchar_fd_count(char c, int fd);
int		check_args(const char *str, va_list input, int pos, char *base);
int		ft_putnbr(int nb);
int		ft_putnbr_u(unsigned int nb);
int		ft_put_base(unsigned int nbr, char *base);
int		ft_putstr_fd_count(char *s, int fd);
int		ft_put_base_l(unsigned long nbr, char *base);

#endif