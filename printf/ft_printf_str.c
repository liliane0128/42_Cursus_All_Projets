/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:15:43 by lilwang           #+#    #+#             */
/*   Updated: 2025/05/27 11:18:22 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_str(char *str)
{
	int	len;

	if (!str)
		str = "(null)";
	len = 0;
	while (str[len])
		len++;
	write(1, str, len);
	return (len);
}
