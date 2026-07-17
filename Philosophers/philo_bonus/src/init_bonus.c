/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:15:33 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:29:04 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

int	init_philos(t_table *table)
{
	int			i;
	long long	start;

	start = current_time_ms();
	table->start_time = start;
	table->philos = malloc(sizeof(t_philo) * table->num_philos);
	if (!table->philos)
		return (1);
	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].last_meal_time = start;
		table->philos[i].pid = -1;
		table->philos[i].table = table;
		i++;
	}
	return (0);
}

int	init_sem(t_table *table)
{
	sem_unlink("forks");
	sem_unlink("finish");
	sem_unlink("state_sem");
	sem_unlink("print_sem");
	table->forks = sem_open("forks", O_CREAT, 0644, table->num_philos);
	table->finish = sem_open("finish", O_CREAT, 0644, 0);
	table->state_sem = sem_open("state_sem", O_CREAT, 0644, 1);
	table->print_sem = sem_open("print_sem", O_CREAT, 0644, 1);
	if (table->forks == SEM_FAILED || table->finish == SEM_FAILED
		|| table->state_sem == SEM_FAILED || table->print_sem == SEM_FAILED)
		return (1);
	return (0);
}
