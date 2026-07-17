/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 08:40:45 by lilwang           #+#    #+#             */
/*   Updated: 2025/07/19 08:49:56 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	find_min_nbr(t_list *lst)
{
	long int	min;
	t_list		*tmp;

	tmp = lst;
	min = LONG_MAX;
	while (tmp)
	{
		if (tmp->index == -1 && tmp->content < min)
			min = tmp->content;
		tmp = tmp->next;
	}
	return (min);
}

static void	assign_next_min_index(t_list *lst, int target_index)
{
	int		min;
	t_list	*tmp;

	min = find_min_nbr(lst);
	tmp = lst;
	while (tmp)
	{
		if (tmp->index == -1 && tmp->content == min)
			break ;
		tmp = tmp->next;
	}
	if (tmp)
		tmp->index = target_index;
}

void	assign_index(t_list *lst)
{
	int	len;
	int	i;

	len = ft_lstsize(lst);
	i = -1;
	while (++i < len)
		assign_next_min_index(lst, i);
}
