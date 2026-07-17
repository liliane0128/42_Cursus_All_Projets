/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_philos_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:35:51 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 19:27:13 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

static void	clean_process(t_philo *ph)
{
	int	i;

	i = 0;
	while (i < ph->table->num_philos)
	{
		sem_post(ph->table->finish);
		i++;
	}
	cleanup(ph->table);
	exit (0);
}

static void	print_death(t_philo *ph)
{
	printf("%lld %d %s\n",
		current_time_ms() - ph->table->start_time, ph->id, "is died");
}

static void	*monitor_philos(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	while (1)
	{
		sem_wait(ph->table->state_sem);
		if (current_time_ms() - ph->last_meal_time > ph->table->time_to_die)
		{
			sem_wait(ph->table->print_sem);
			ph->table->someone_died = 1;
			print_death(ph);
			sem_post(ph->table->state_sem);
			clean_process(ph);
		}
		sem_post(ph->table->state_sem);
		ft_usleep(1);
	}
	return (NULL);
}

static int	philo_routine(t_philo *ph)
{
	if (pthread_create(&ph->monitor_thread, NULL, monitor_philos, ph) != 0)
		return (error_msg("pthread_create failed\n"));
	pthread_detach(ph->monitor_thread);
	if (ph->id % 2 == 0)
		ft_usleep(1);
	while (!check_state(ph))
	{
		philo_eat(ph);
		sem_wait(ph->table->state_sem);
		if (ph->meals_eaten == ph->table->must_eat_count)
			sem_post(ph->table->finish);
		sem_post(ph->table->state_sem);
		philo_sleep(ph);
		philo_think(ph);
		usleep(100);
	}
	return (0);
}

int	start_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].pid = fork();
		if (table->philos[i].pid < 0)
			return (1);
		if (table->philos[i].pid == 0)
		{
			philo_routine(&table->philos[i]);
			exit(0);
		}
		i++;
	}
	terminate_processes(table);
	cleanup(table);
	return (0);
}
