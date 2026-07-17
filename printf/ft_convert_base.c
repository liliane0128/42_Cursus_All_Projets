/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:59:44 by lilwang           #+#    #+#             */
/*   Updated: 2025/05/27 11:12:36 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_convert_base(unsigned long n, char *base, int base_len, char *buf)
{
	int		i;

	i = 0;
	if (n == 0)
	{
		buf[0] = base[0];
		buf[1] = '\0';
		return (1);
	}
	while (n > 0)
	{
		buf[i++] = base[n % base_len];
		n /= base_len;
	}
	buf[i] = '\0';
	return (i);
}

void	reverse_str(char *str, int len)
{
	int		j;
	int		k;
	char	temp;

	j = 0;
	k = len - 1;
	while (j < k)
	{
		temp = str[j];
		str[j] = str[k];
		str[k] = temp;
		j++;
		k--;
	}
}

int	my_putnbr_base(unsigned long n, char *base, int base_len)
{
	char	buf[65];
	int		len;

	len = ft_convert_base(n, base, base_len, buf);
	reverse_str(buf, len);
	write (1, buf, len);
	return (len);
}
