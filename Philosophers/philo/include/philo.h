/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:40:13 by lilwang           #+#    #+#             */
/*   Updated: 2025/09/04 16:34:38 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include <string.h>

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	long long		last_meal_time;
	int				meals_eaten;
	pthread_t		thread_id;
	struct s_rules	*rules;
}				t_philo;

typedef struct s_rules
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				someone_died;
	long long		start_time;
	int				all_ate;
	pthread_t		monitor;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	state_lock;
	t_philo			*philos;
}				t_rules;

/*parsing*/
int			parse_args(int ac, char **av, t_rules *rules);

/*init*/
int			init_philos(t_rules *rules);
int			init_mutex(t_rules *rules);

/*start_philos*/
int			start_philos(t_rules *rules);

/*philosopher*/
void		philo_think(t_philo *ph);
void		philo_eat(t_philo *ph);
void		philo_sleep(t_philo *ph);

/*monitor.c*/
void		*monitor_philos(void *arg);

/*utils*/
long long	current_time_ms(void);
int			error_msg(const char *msg);
int			ft_usleep(int millisecondes);
void		print_status(t_philo *ph, char *msg);
int			check_state(t_philo *ph);

#endif
