/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hidsaid <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:34:44 by hidsaid           #+#    #+#             */
/*   Updated: 2025/04/24 12:34:45 by hidsaid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_sleep(t_philo *philo, long span)
{
	long	start;
	long	current_time;

	start = time_in_ms();
	current_time = 0;
	while (current_time < span)
	{
		pthread_mutex_lock(&philo->args->mutexes[DEATH_MUTEX]);
		if (philo->args->someone_died)
		{
			pthread_mutex_unlock(&philo->args->mutexes[DEATH_MUTEX]);
			return (1);
		}
		pthread_mutex_unlock(&philo->args->mutexes[DEATH_MUTEX]);
		usleep(100);
		current_time = time_in_ms() - start;
	}
	return (0);
}

void	eat(t_philo *philo, int first_fork, int second_fork)
{
	if (check_end_simulation(philo))
		return ;
	pthread_mutex_lock(&philo->args->forks[first_fork]);
	print_action(philo, FORK);
	pthread_mutex_lock(&philo->args->forks[second_fork]);
	print_action(philo, FORK);
	pthread_mutex_lock(&philo->args->mutexes[MEAL_MUTEX]);
	philo->last_meal = time_in_ms();
	pthread_mutex_unlock(&philo->args->mutexes[MEAL_MUTEX]);
	print_action(philo, EAT);
	if (philo_sleep(philo, philo->args->time_to_eat))
	{
		pthread_mutex_unlock(&philo->args->forks[first_fork]);
		pthread_mutex_unlock(&philo->args->forks[second_fork]);
		return ;
	}
	pthread_mutex_unlock(&philo->args->forks[first_fork]);
	pthread_mutex_unlock(&philo->args->forks[second_fork]);
}

void	print_action(t_philo *philo, int action)
{
	long	timestamp;

	if (check_end_simulation(philo))
		return ;
	timestamp = get_timestamp(philo, START_TIME_MUTEX);
	if (action == THINK)
		print_msg(philo, timestamp, "thinking");
	else if (action == FORK)
		print_msg(philo, timestamp, "has taken a fork");
	else if (action == EAT)
	{
		pthread_mutex_lock(&philo->args->mutexes[MEAL_MUTEX]);
		(philo->meal_count)++;
		pthread_mutex_unlock(&philo->args->mutexes[MEAL_MUTEX]);
		print_msg(philo, timestamp, "eating");
	}
	else if (action == SLEEP)
		print_msg(philo, timestamp, "sleeping");
}

void	print_msg(t_philo *philo, long timestamp, char *str)
{
	pthread_mutex_lock(&philo->args->mutexes[PRINT_MUTEX]);
	printf("%ld Philo %d %s\n", timestamp, philo->id + 1, str);
	pthread_mutex_unlock(&philo->args->mutexes[PRINT_MUTEX]);
}

void	clean_up(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < MUTEX_COUNT)
		pthread_mutex_destroy(&philo->args->mutexes[i++]);
	i = 0;
	while (i < philo->args->nbr_philos)
		pthread_mutex_destroy(&philo->args->forks[i++]);
	free(philo->args->forks);
	free(philo);
}
