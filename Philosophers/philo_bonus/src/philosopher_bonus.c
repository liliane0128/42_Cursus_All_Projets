/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:22:52 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 18:37:12 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	philo_eat(t_philo *ph)
{
	sem_wait(ph->table->forks);
	print_status(ph, "has taken a fork");
	sem_wait(ph->table->forks);
	print_status(ph, "has taken a fork");
	print_status(ph, "is eating");
	sem_wait(ph->table->state_sem);
	ph->last_meal_time = current_time_ms();
	ph->meals_eaten++;
	sem_post(ph->table->state_sem);
	ft_usleep(ph->table->time_to_eat);
	sem_post(ph->table->forks);
	sem_post(ph->table->forks);
}

void	philo_sleep(t_philo *ph)
{
	print_status(ph, "is sleeping");
	ft_usleep(ph->table->time_to_sleep);
}

static long	get_urgency(t_philo *ph)
{
	long	now;
	long	time_since_meal;

	now = current_time_ms();
	time_since_meal = now - ph->last_meal_time;
	return (ph->table->time_to_die - time_since_meal);
}

void	philo_think(t_philo *ph)
{
	long	start_time;
	long	duration;
	long	urgency;
	long	safe_margin;

	start_time = current_time_ms();
	ph->last_meal_time = ph->last_meal_time;
	safe_margin = ph->table->time_to_eat + 5;
	while (1)
	{
		duration = current_time_ms() - start_time;
		urgency = get_urgency(ph);
		if (urgency <= safe_margin || duration > 100)
			break ;
		usleep(200);
	}
	print_status(ph, "is thinking");
}
