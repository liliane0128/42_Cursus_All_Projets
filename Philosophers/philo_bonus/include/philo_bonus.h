/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:05:24 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/11 19:30:34 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include <string.h>
# include <signal.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_philo
{
	int				id;
	long long		last_meal_time;
	int				meals_eaten;
	pid_t			pid;
	pthread_t		monitor_thread;
	struct s_table	*table;
}				t_philo;

typedef struct s_table
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				someone_died;
	long long		start_time;
	sem_t			*forks;
	sem_t			*finish;
	sem_t			*state_sem;
	sem_t			*print_sem;
	t_philo			*philos;
}				t_table;

/*parsing_bonus*/
int			parse_args(int ac, char **av, t_table *table);

/*init_bonus*/
int			init_philos(t_table *table);
int			init_sem(t_table *table);

/*start_philos_bonus*/
int			start_philos(t_table *table);

/*philosopher_bonus*/
void		philo_think(t_philo *ph);
void		philo_eat(t_philo *ph);
void		philo_sleep(t_philo *ph);

/*end_process_bonus*/
void		terminate_processes(t_table *table);
void		cleanup(t_table *table);

/*utils_bonus*/
long long	current_time_ms(void);
int			error_msg(const char *msg);
int			ft_usleep(int millisecondes);
void		print_status(t_philo *ph, char *msg);
int			check_state(t_philo *ph);

#endif
