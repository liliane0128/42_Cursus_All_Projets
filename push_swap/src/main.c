/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 08:40:58 by lilwang           #+#    #+#             */
/*   Updated: 2025/07/19 08:49:58 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int ac, char **av)
{
	t_list	*a;
	t_list	*b;

	a = NULL;
	b = NULL;
	if (ac < 2)
		return (0);
	if (!parse_args(ac, av, &a))
	{
		write(2, "Error\n", 6);
		free_stack(&a);
		return (1);
	}
	if (is_sorted(a))
	{
		free_stack(&a);
		return (0);
	}
	assign_index(a);
	sort_chunks(&a, &b);
	free_stack(&a);
	return (0);
}
