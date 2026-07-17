/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:19:36 by lilwang           #+#    #+#             */
/*   Updated: 2025/05/29 09:21:38 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	my_printf_int(int n)
{
	int		len;
	long	num;

	len = 0;
	num = (long)n;
	if (num < 0)
	{
		len += ft_printf_char('-');
		num = -num;
	}
	len += my_putnbr_base((unsigned long)num, "0123456789", 10);
	return (len);
}

int	my_printf_uint(unsigned int n)
{
	return (my_putnbr_base(n, "0123456789", 10));
}

int	my_printf_hex_upper(unsigned int n)
{
	return (my_putnbr_base(n, "0123456789ABCDEF", 16));
}

int	my_printf_hex_lower(unsigned int n)
{
	return (my_putnbr_base(n, "0123456789abcdef", 16));
}

int	my_printf_ptr(void *ptr)
{
	unsigned long	addr;
	int				len;

	if (!ptr)
		return (ft_printf_str("(nil)"));
	addr = (unsigned long)ptr;
	len = 0;
	len += ft_printf_str("0x");
	len += my_putnbr_base(addr, "0123456789abcdef", 16);
	return (len);
}
