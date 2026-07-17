/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:37:33 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 15:37:04 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	cleanup(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philos)
		pthread_mutex_destroy(&rules->forks[i++]);
	pthread_mutex_destroy(&rules->print_lock);
	pthread_mutex_destroy(&rules->meal_lock);
	pthread_mutex_destroy(&rules->state_lock);
	free(rules->forks);
	free(rules->philos);
}

static int	setup_rules(int ac, char **av, t_rules *rules)
{
	if (parse_args(ac, av, rules))
		return (error_msg("invalid argument"));
	if (init_philos(rules) || init_mutex(rules))
		return (error_msg("initialization failed"));
	return (0);
}

static int	run_simulation(t_rules *rules)
{
	int	i;

	if (start_philos(rules))
	{
		cleanup(rules);
		return (error_msg("thread creation failed"));
	}
	i = 0;
	while (i < rules->num_philos)
	{
		pthread_join(rules->philos[i].thread_id, NULL);
		i++;
	}
	pthread_join(rules->monitor, NULL);
	cleanup(rules);
	return (0);
}

int	main(int ac, char **av)
{
	t_rules	rules;

	memset(&rules, 0, sizeof(t_rules));
	if (setup_rules(ac, av, &rules))
		return (1);
	if (run_simulation(&rules))
		return (1);
	return (0);
}
