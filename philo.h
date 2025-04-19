#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_args
{
	int				nbr_philos;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int				must_eat_count;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	meal_mutex;
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
enum e_action
{
	THINK,
	LEFT_FORK,
	RIGHT_FORK,
	EAT,
	SLEEP
};

// Error Messages
# define ERR_ARG_COUNT "Error: Wrong number of arguments!\n"
# define ERR_NON_NUMERIC "Error: Non-numeric argument!\n"
# define ERR_INVALID_VALUES "Error: Invalid argument values!\n"
# define ERR_NOT_INT "Error: Argument is not an integer!\n"
# define ERR_ALLOC "Error: allocation failed\n"
#include <sys/time.h>

void				pars_args(t_args *philo, int argc, char *argv[]);
t_philo				*init_philo(t_args *args, t_philo *philo);

#endif