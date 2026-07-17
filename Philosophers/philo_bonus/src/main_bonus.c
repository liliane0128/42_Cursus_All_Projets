/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:10:30 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:44:34 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

static int	init_and_run(int ac, char **av, t_table *table)
{
	if (parse_args(ac, av, table))
		return (error_msg("invalid argument"));
	if (init_sem(table))
		return (error_msg("semaphore creation failed"));
	if (init_philos(table))
		return (error_msg("initialization of philosophers failed"));
	if (start_philos(table))
	{
		cleanup(table);
		return (error_msg("process creation failed"));
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_table	table;

	memset(&table, 0, sizeof(t_table));
	if (init_and_run(ac, av, &table))
		return (1);
	return (0);
}
