/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_philos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:37:48 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:37:17 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	*routine(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (ph->id % 2 == 0)
		ft_usleep(1);
	pthread_mutex_lock(&ph->rules->meal_lock);
	ph->last_meal_time = current_time_ms();
	pthread_mutex_unlock(&ph->rules->meal_lock);
	while (!check_state(ph))
	{
		philo_eat(ph);
		philo_sleep(ph);
		philo_think(ph);
	}
	return (NULL);
}

int	start_philos(t_rules *rules)
{
	int	i;

	rules->start_time = current_time_ms();
	if (rules->num_philos == 1)
	{
		print_status(&rules->philos[0], "has taken a fork");
		ft_usleep(rules->time_to_die);
		print_status(&rules->philos[0], "died");
		return (0);
	}
	i = 0;
	while (i < rules->num_philos)
	{
		if (pthread_create(&rules->philos[i].thread_id, NULL, routine,
				&rules->philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&rules->monitor, NULL, monitor_philos, rules) != 0)
		return (1);
	return (0);
}
