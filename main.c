/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hidsaid <hidsaid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:46:14 by hidsaid           #+#    #+#             */
/*   Updated: 2025/04/24 10:34:41 by hidsaid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->args->nbr_philos == 1)
		return (print_action(philo, FORK), NULL);
	while (1)
	{
		if (philo->args->nbr_philos % 2)
		{
			if (philo_sleep(philo, philo->args->time_to_eat))
				return (NULL);
		}
		if (check_end_simulation(philo))
			return (NULL);
		print_action(philo, THINK);
		if (philo->id % 2)
			eat(philo, philo->left, philo->right);
		else
			eat(philo, philo->right, philo->left);
		print_action(philo, SLEEP);
		if (philo_sleep(philo, philo->args->time_to_sleep))
			return (NULL);
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	while (1)
	{
		i = 0;
		while (i < philo->args->nbr_philos)
		{
			if (check_philo_status(philo, i++))
				return (NULL);
		}
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_args		args;
	t_philo		*philo;
	pthread_t	tid;
	int			i;

	if (pars_args(&args, argc, argv))
		return (1);
	philo = NULL;
	philo = init_philo(&args, philo);
	i = 0;
	while (i < args.nbr_philos)
	{
		pthread_create(&philo[i].tid, NULL, routine, &philo[i]);
		i++;
	}
	i = 0;
	pthread_create(&tid, NULL, monitor, philo);
	while (i < args.nbr_philos)
		pthread_join(philo[i++].tid, NULL);
	pthread_join(tid, NULL);
	clean_up(philo);
	return (0);
}
