/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:37:21 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 18:56:35 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	philo_eat(t_philo *ph)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	first_fork = &ph->rules->forks[ph->left_fork];
	second_fork = &ph->rules->forks[ph->right_fork];
	if (ph->id % 2 == 0)
	{
		first_fork = second_fork;
		second_fork = &ph->rules->forks[ph->left_fork];
	}
	pthread_mutex_lock(first_fork);
	print_status(ph, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(ph, "has taken a fork");
	print_status(ph, "is eating");
	pthread_mutex_lock(&ph->rules->meal_lock);
	ph->last_meal_time = current_time_ms();
	ph->meals_eaten++;
	pthread_mutex_unlock(&ph->rules->meal_lock);
	ft_usleep(ph->rules->time_to_eat);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

void	philo_sleep(t_philo *ph)
{
	print_status(ph, "is sleeping");
	ft_usleep(ph->rules->time_to_sleep);
}

void	philo_think(t_philo *ph)
{
	long long	now;
	long long	last_meal;
	long long	remaining_time;
	long long	duration;

	now = current_time_ms();
	pthread_mutex_lock(&ph->rules->meal_lock);
	last_meal = ph->last_meal_time;
	pthread_mutex_unlock(&ph->rules->meal_lock);
	remaining_time = ph->rules->time_to_die - (now - last_meal) - 10;
	if (remaining_time < 0)
		remaining_time = 0;
	duration = 50 + (ph->id * 37 + now % 100) % 150;
	if (duration > remaining_time)
		duration = remaining_time;
	print_status(ph, "is thinking");
	if (remaining_time > 20)
		ft_usleep(duration);
}
/*
 * philo_think:
 * Makes the philosopher "think" for a short, random-ish duration(50-199ms).
 * The duration depends on the philosopher's ID and current time,
 * ensuring at least 50ms and never exceeding the remaining time
 * before they would die (time_to_die - last_meal - 10ms).
 * If the remaining time is less than 20ms, thinking is skipped to avoid death.
*/