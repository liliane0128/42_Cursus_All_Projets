/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 11:28:11 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/19 18:28:27 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t new_size)
{
	void	*tmp;
	size_t	i;

	tmp = malloc(new_size);
	if (!tmp)
		return (NULL);
	if (ptr)
	{
		i = 0;
		while (i < new_size)
		{
			((char *)tmp)[i] = ((char *)ptr)[i];
			i++;
		}
		free(ptr);
	}
	return (tmp);
}
