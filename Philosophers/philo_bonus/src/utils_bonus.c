/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:33:45 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/14 17:15:54 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo_bonus.h"

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
	while ((current_time_ms() - start) < millisecondes)
		usleep(100);
	return (0);
}

void	print_status(t_philo *ph, char *msg)
{
	sem_wait(ph->table->print_sem);
	if (!ph->table->someone_died)
		printf("%lld %d %s\n",
			current_time_ms() - ph->table->start_time, ph->id, msg);
	sem_post(ph->table->print_sem);
}

int	check_state(t_philo *ph)
{
	int	ret;

	sem_wait(ph->table->state_sem);
	ret = (ph->table->someone_died == 1);
	sem_post(ph->table->state_sem);
	return (ret);
}
