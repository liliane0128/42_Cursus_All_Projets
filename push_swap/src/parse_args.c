/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 08:41:22 by lilwang           #+#    #+#             */
/*   Updated: 2025/07/19 08:49:40 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	is_number(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	is_duplicate(t_list	**a, long int value)
{
	t_list	*tmp;

	tmp = *a;
	while (tmp)
	{
		if (tmp->content == value)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static int	parse_and_add_values(char *arg, t_list **a)
{
	char		**nums;
	int			i;
	long int	value;

	nums = ft_split(arg, ' ');
	if (!nums)
		return (0);
	i = 0;
	while (nums[i])
	{
		if (!is_number(nums[i]))
			return (free_split(nums), 0);
		value = ft_atol(nums[i]);
		if (is_duplicate(a, value) || value < INT_MIN || value > INT_MAX)
			return (free_split(nums), 0);
		if (!lst_add_back(a, lst_new_node(value)))
			return (free_split(nums), 0);
		i++;
	}
	free_split(nums);
	return (1);
}

int	parse_args(int ac, char **av, t_list **a)
{
	int	i;

	i = 0;
	while (++i < ac)
	{
		if (!parse_and_add_values(av[i], a))
			return (0);
	}
	return (1);
}
