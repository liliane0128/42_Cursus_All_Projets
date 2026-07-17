/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 08:41:36 by lilwang           #+#    #+#             */
/*   Updated: 2025/07/19 21:35:19 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	find_index_pos(t_list *stack, int obj_index)
{
	int		pos;
	t_list	*tmp;

	pos = 0;
	tmp = stack;
	while (tmp)
	{
		if (tmp->index == obj_index)
			return (pos);
		pos++;
		tmp = tmp->next;
	}
	return (-1);
}

int	is_sorted(t_list *stack)
{
	t_list	*tmp;

	tmp = stack;
	while (tmp && tmp->next)
	{
		if (tmp->content > tmp->next->content)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

void	sort_3(t_list **stack_a)
{
	int	a;
	int	b;
	int	c;

	if (is_sorted(*stack_a))
		return ;
	a = (*stack_a)->index;
	b = (*stack_a)->next->index;
	c = ft_lstlast(*stack_a)->index;
	if (a > b && b < c && a < c)
		sa(stack_a);
	else if (a > b && b > c)
	{
		sa(stack_a);
		rra(stack_a);
	}
	else if (a > b && b < c && a > c)
		ra(stack_a);
	else if (a < b && b > c && a < c)
	{
		sa(stack_a);
		ra(stack_a);
	}
	else if (a < b && b > c && a > c)
		rra(stack_a);
}

void	sort_5(t_list **stack_a, t_list **stack_b)
{
	int	size;
	int	pushed;
	int	idx;

	size = ft_lstsize(*stack_a);
	pushed = 0;
	while (ft_lstsize(*stack_a) > 3)
	{
		idx = (*stack_a)->index;
		if (idx == 0 || (size == 5 && idx == 1))
		{
			pb(stack_a, stack_b);
			pushed++;
		}
		else
			ra(stack_a);
	}
	sort_3(stack_a);
	if (pushed == 2 && (*stack_b)->index < (*stack_b)->next->index)
		sb(stack_b);
	while (pushed--)
		pa(stack_a, stack_b);
}
