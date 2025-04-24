/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hidsaid <hidsaid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:03:28 by hidsaid           #+#    #+#             */
/*   Updated: 2025/04/24 12:32:54 by hidsaid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

enum				e_mutex
{
	PRINT_MUTEX,
	MEAL_MUTEX,
	DEATH_MUTEX,
	START_TIME_MUTEX,
	MUTEX_COUNT
};

typedef struct s_args
{
	int				nbr_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	long			start_time;
	int				someone_died;
	pthread_mutex_t	*forks;
	pthread_mutex_t	mutexes[MUTEX_COUNT];
}					t_args;

typedef struct philo
{
	pthread_t		tid;
	int				id;
	int				left;
	int				right;
	int				meal_count;
	long			last_meal;
	t_args			*args;
}					t_philo;

// actions
enum				e_action
{
	THINK,
	FORK,
	EAT,
	SLEEP,
	DIED
};

// Error Messages
# define ERR_ARG_COUNT "Error: Wrong number of arguments!\n"
# define ERR_INVALID_VALUES "Error: Invalid argument values!\n"
# define ERR_ALLOC "Error: allocation failed\n"

// parsing
int					pars_args(t_args *philo, int argc, char *argv[]);
t_philo				*init_philo(t_args *args, t_philo *philo);

// philos routine
int					philo_sleep(t_philo *philo, long span);
void				eat(t_philo *philo, int first_fork, int second_fork);
void				print_action(t_philo *philo, int action);
void				print_msg(t_philo *philo, long timestamp, char *str);
void				clean_up(t_philo *philo);
// philo monitor
int					check_philo_status(t_philo *philo, int i);
int					check_eaten_enough(t_philo *philo);
int					check_end_simulation(t_philo *philo);
long				time_in_ms(void);
long				get_timestamp(t_philo *philo, int mutex);

#endif
