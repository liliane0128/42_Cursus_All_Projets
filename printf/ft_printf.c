/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 09:23:17 by lilwang           #+#    #+#             */
/*   Updated: 2025/11/27 17:41:29 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	handle_conversion(char c, va_list ap)
{
	if (c == 'c')
		return (ft_printf_char(va_arg(ap, int)));
	else if (c == 's')
		return (ft_printf_str(va_arg(ap, char *)));
	else if (c == 'p')
		return (my_printf_ptr(va_arg(ap, void *)));
	else if (c == 'd' || c == 'i')
		return (my_printf_int(va_arg(ap, int)));
	else if (c == 'u')
		return (my_printf_uint(va_arg(ap, unsigned int)));
	else if (c == 'X')
		return (my_printf_hex_upper(va_arg(ap, unsigned int)));
	else if (c == 'x')
		return (my_printf_hex_lower(va_arg(ap, unsigned int)));
	else if (c == '%')
		return (ft_printf_char('%'));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		i;
	int		count;

	if (!format)
		return (-1);
	i = 0;
	count = 0;
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			count += handle_conversion(format[i], ap);
		}
		else
			count += ft_printf_char(format[i]);
		i++;
	}
	va_end(ap);
	return (count);
}