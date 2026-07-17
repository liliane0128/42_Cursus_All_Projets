/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:14:45 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:29:10 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

static int	is_all_digits(const char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_atoi_safe(const char *s)
{
	long long	num;

	num = 0;
	if (!s || !*s)
		return (-1);
	if (!is_all_digits(s))
		return (-1);
	while (*s >= '0' && *s <= '9')
	{
		num = 10 * num + (*s - '0');
		if (num > INT_MAX)
			return (-1);
		s++;
	}
	return ((int)num);
}

int	parse_args(int ac, char **av, t_table *table)
{
	if (ac != 5 && ac != 6)
		return (1);
	table->num_philos = ft_atoi_safe(av[1]);
	table->time_to_die = ft_atoi_safe(av[2]);
	table->time_to_eat = ft_atoi_safe(av[3]);
	table->time_to_sleep = ft_atoi_safe(av[4]);
	if (ac == 6)
		table->must_eat_count = ft_atoi_safe(av[5]);
	else
		table->must_eat_count = -1;
	if (table->num_philos <= 0 || table->time_to_die <= 0
		|| table->time_to_eat <= 0 || table->time_to_sleep <= 0
		|| (ac == 6 && table->must_eat_count <= 0))
		return (1);
	table->someone_died = 0;
	return (0);
}
