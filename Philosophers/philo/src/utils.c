/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:37:41 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:37:21 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

long long	current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

int	error_msg(const char *msg)
{
	printf("Error: %s\n", msg);
	return (1);
}

int	ft_usleep(int millisecondes)
{
	long long	start;

	start = current_time_ms();
	while (current_time_ms() - start < millisecondes)
		usleep(100);
	return (0);
}

void	print_status(t_philo *ph, char *msg)
{
	pthread_mutex_lock(&ph->rules->print_lock);
	if (!check_state(ph))
		printf("%lld %d %s\n",
			current_time_ms() - ph->rules->start_time, ph->id, msg);
	pthread_mutex_unlock(&ph->rules->print_lock);
}

int	check_state(t_philo *ph)
{
	int	dead;

	pthread_mutex_lock(&ph->rules->state_lock);
	dead = (ph->rules->someone_died || ph->rules->all_ate);
	pthread_mutex_unlock(&ph->rules->state_lock);
	return (dead);
}
/*
 * ft_usleep 
 * - custom sleep function in milliseconds
 * This function pauses the current thread for approximately the given
 * number of milliseconds. It splits the sleep into small 100 microsecond
 * intervals to allow the CPU to be shared with other threads and to
 * let a monitor thread check shared conditions (like someone_died)
 * frequently, preventing blocking the program for too long.
 */