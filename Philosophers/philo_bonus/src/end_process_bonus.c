/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_process_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:58:48 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:42:25 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

void	terminate_processes(t_table *table)
{
	int	i;
	int	status;

	i = 0;
	while (i < table->num_philos)
	{
		sem_wait(table->finish);
		i++;
	}
	i = 0;
	while (i < table->num_philos)
	{
		kill(table->philos[i].pid, SIGKILL);
		waitpid(table->philos[i].pid, &status, 0);
		i++;
	}
}

void	cleanup(t_table *table)
{
	if (table)
	{
		if (table->forks)
			sem_close(table->forks);
		if (table->finish)
			sem_close(table->finish);
		if (table->state_sem)
			sem_close(table->state_sem);
		if (table->print_sem)
			sem_close(table->print_sem);
		sem_unlink("forks");
		sem_unlink("finish");
		sem_unlink("state_sem");
		sem_unlink("print_sem");
		if (table->philos)
			free(table->philos);
	}
}
