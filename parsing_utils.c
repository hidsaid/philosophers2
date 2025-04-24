/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hidsaid <hidsaid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:46:18 by hidsaid           #+#    #+#             */
/*   Updated: 2025/04/24 10:27:30 by hidsaid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_is_number(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long long	ft_atoi(const char *s)
{
	int			sign;
	long long	res;
	int			i;

	sign = 1;
	res = 0;
	i = 0;
	while (s[i] == 32 || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + s[i++] - '0';
		if ((res * sign) > LONG_MAX)
			return (0);
	}
	return (res * sign);
}

int	pars_args(t_args *args, int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc < 5 || argc > 6)
		return (printf(ERR_ARG_COUNT), 1);
	args->nbr_philos = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	args->someone_died = 0;
	(argc == 6) && (args->must_eat_count = ft_atoi(argv[5]));
	if (argc != 6)
		args->must_eat_count = -1;
	while (i < argc)
	{
		if (!check_is_number(argv[i++]))
			return (printf(ERR_INVALID_VALUES), 1);
	}
	if (args->nbr_philos < 1 || args->time_to_die <= 0 || args->time_to_eat <= 0
		|| args->time_to_sleep <= 0 || (argc == 6 && args->must_eat_count <= 0))
		return (printf(ERR_INVALID_VALUES), 1);
	return (0);
}

static void	init_mutexes(t_args *args)
{
	int	i;

	i = 0;
	while (i < MUTEX_COUNT)
	{
		if (pthread_mutex_init(&args->mutexes[i++], NULL))
			return ((void)(printf(ERR_ALLOC)));
	}
}

t_philo	*init_philo(t_args *args, t_philo *philo)
{
	int	i;

	philo = malloc(sizeof(t_philo) * args->nbr_philos);
	args->forks = malloc(sizeof(pthread_mutex_t) * args->nbr_philos);
	if (!philo || !args->forks)
		return (printf(ERR_ALLOC), exit(1), NULL);
	args->start_time = time_in_ms();
	init_mutexes(args);
	i = 0;
	while (i < args->nbr_philos)
	{
		philo[i].last_meal = args->start_time;
		philo[i].meal_count = 0;
		philo[i].args = args;
		philo[i].id = i;
		if (i == 0)
			philo[i].left = philo[i].args->nbr_philos - 1;
		else
			philo[i].left = i - 1;
		philo[i].right = i;
		if (pthread_mutex_init(&args->forks[i++], NULL))
			return (printf(ERR_ALLOC), NULL);
	}
	return (philo);
}
