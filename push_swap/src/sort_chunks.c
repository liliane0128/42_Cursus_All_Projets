/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_chunks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 08:41:32 by lilwang           #+#    #+#             */
/*   Updated: 2025/07/24 10:49:22 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_chunks(t_list **a, t_list **b)
{
	int	size;
	int	chunk_size;

	size = ft_lstsize(*a);
	if (size <= 1)
		return ;
	else if (size == 2)
	{
		if ((*a)->index > (*a)->next->index)
			sa(a);
	}
	else if (size == 3)
		sort_3(a);
	else if (size <= 5)
		sort_5(a, b);
	else
	{
		chunk_size = size / 20 + 6;
		push_chunks_to_b(a, b, chunk_size);
		push_back_to_a(a, b);
	}
}

void	push_chunks_to_b(t_list **a, t_list **b, int chunk_size)
{
	int	i;

	i = 0;
	while (*a)
	{
		if ((*a)->index <= i + chunk_size)
		{
			pb(a, b);
			if (*b && (*b)->index < i)
				rb(b);
			i++;
		}
		else
			ra(a);
	}
}

void	push_back_to_a(t_list **a, t_list **b)
{
	int	max_index;
	int	pos;

	while (*b)
	{
		max_index = ft_lstsize(*b) - 1;
		pos = find_index_pos(*b, max_index);
		if (pos <= ft_lstsize(*b) / 2)
		{
			while ((*b)->index != max_index)
				rb(b);
		}
		else
		{
			while ((*b)->index != max_index)
				rrb(b);
		}
		pa(a, b);
	}
}
