/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:08:19 by lilwang           #+#    #+#             */
/*   Updated: 2025/05/29 09:22:53 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		ft_convert_base(unsigned long n, char *base, int base_len, char *buf);
void	reverse_str(char *str, int len);
int		ft_printf_char(char c);
int		ft_printf_str(char *str);
int		my_putnbr_base(unsigned long n, char *base, int base_len);
int		my_printf_int(int n);
int		my_printf_uint(unsigned int n);
int		my_printf_hex_upper(unsigned int n);
int		my_printf_hex_lower(unsigned int n);
int		my_printf_ptr(void *ptr);
int		handle_conversion(char c, va_list args);
int		ft_printf(const char *format, ...);

#endif
