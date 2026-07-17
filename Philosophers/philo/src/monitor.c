/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:38:04 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:37:57 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	check_death(t_philo *ph)
{
	long long	now;

	pthread_mutex_lock(&ph->rules->meal_lock);
	now = current_time_ms();
	if (now - ph->last_meal_time > ph->rules->time_to_die)
	{
		pthread_mutex_unlock(&ph->rules->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&ph->rules->meal_lock);
	return (0);
}

static int	philo_dead(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philos)
	{
		if (check_death(&rules->philos[i]))
		{
			pthread_mutex_lock(&rules->state_lock);
			if (!rules->someone_died)
			{
				pthread_mutex_unlock(&rules->state_lock);
				print_status(&rules->philos[i], "died");
				pthread_mutex_lock(&rules->state_lock);
				rules->someone_died = 1;
			}
			pthread_mutex_unlock(&rules->state_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	all_ate_enough(t_rules *rules)
{
	int	i;
	int	total;

	i = 0;
	total = 0;
	while (i < rules->num_philos)
	{
		pthread_mutex_lock(&rules->meal_lock);
		if (rules->must_eat_count > 0
			&& rules->philos[i].meals_eaten >= rules->must_eat_count)
			total++;
		pthread_mutex_unlock(&rules->meal_lock);
		i++;
	}
	if (total == rules->num_philos)
	{
		pthread_mutex_lock(&rules->state_lock);
		rules->all_ate = 1;
		pthread_mutex_unlock(&rules->state_lock);
		return (1);
	}
	return (0);
}

void	*monitor_philos(void *arg)
{
	t_rules	*rules;

	rules = (t_rules *)arg;
	while (1)
	{
		if (philo_dead(rules) || all_ate_enough(rules))
			break ;
		ft_usleep(1);
	}
	return (NULL);
}
