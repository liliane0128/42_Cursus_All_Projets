/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:37:37 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:36:58 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	init_philos(t_rules *rules)
{
	int			i;
	long long	start;

	start = current_time_ms();
	rules->start_time = start;
	rules->philos = malloc(sizeof(t_philo) * rules->num_philos);
	if (!rules->philos)
		return (1);
	i = 0;
	while (i < rules->num_philos)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].left_fork = i;
		rules->philos[i].right_fork = (i + 1) % rules->num_philos;
		rules->philos[i].last_meal_time = start;
		rules->philos[i].meals_eaten = 0;
		rules->philos[i].rules = rules;
		i++;
	}
	return (0);
}

int	init_mutex(t_rules *rules)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philos);
	if (!rules->forks)
		return (1);
	i = 0;
	while (i < rules->num_philos)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&rules->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&rules->meal_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&rules->state_lock, NULL) != 0)
		return (1);
	return (0);
}
