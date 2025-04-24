/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hidsaid <hidsaid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:34:22 by hidsaid           #+#    #+#             */
/*   Updated: 2025/04/24 12:49:03 by hidsaid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_status(t_philo *philo, int i)
{
	long	timestamp;
	int		died;

	pthread_mutex_lock(&philo[i].args->mutexes[MEAL_MUTEX]);
	timestamp = time_in_ms() - philo[i].last_meal;
	pthread_mutex_unlock(&philo[i].args->mutexes[MEAL_MUTEX]);
	pthread_mutex_lock(&philo[i].args->mutexes[DEATH_MUTEX]);
	died = timestamp >= philo[i].args->time_to_die;
	pthread_mutex_unlock(&philo[i].args->mutexes[DEATH_MUTEX]);
	if (died || check_eaten_enough(philo))
	{
		pthread_mutex_lock(&philo[i].args->mutexes[DEATH_MUTEX]);
		philo[i].args->someone_died = 1;
		pthread_mutex_unlock(&philo[i].args->mutexes[DEATH_MUTEX]);
		timestamp = get_timestamp(philo, START_TIME_MUTEX);
		if (check_eaten_enough(philo) == 0)
			print_msg(&philo[i], timestamp, "died");
		return (1);
	}
	return (0);
}

int	check_eaten_enough(t_philo *philo)
{
	int	eaten_enough;

	eaten_enough = 0;
	pthread_mutex_lock(&philo->args->mutexes[MEAL_MUTEX]);
	eaten_enough = philo->meal_count == philo->args->must_eat_count;
	pthread_mutex_unlock(&philo->args->mutexes[MEAL_MUTEX]);
	return (eaten_enough);
}

int	check_end_simulation(t_philo *philo)
{
	int	died;

	pthread_mutex_lock(&philo->args->mutexes[DEATH_MUTEX]);
	died = philo->args->someone_died;
	pthread_mutex_unlock(&philo->args->mutexes[DEATH_MUTEX]);
	if (died || check_eaten_enough(philo))
		return (1);
	return (0);
}

long	time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	get_timestamp(t_philo *philo, int mutex)
{
	long	timestamp;

	pthread_mutex_lock(&philo->args->mutexes[mutex]);
	timestamp = time_in_ms() - philo->args->start_time;
	pthread_mutex_unlock(&philo->args->mutexes[mutex]);
	return (timestamp);
}
