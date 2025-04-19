#include "philo.h"

long time_in_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int check_died(t_philo *philo)
{
	int now = time_in_ms();
	int elapsed;
	if (philo->last_meal == 0)
		return (1);
	elapsed = now - philo->last_meal;
	if (elapsed >= philo->args->time_to_die)
	{
		printf("Philosopher %d died\n", philo->id);
		return (exit(0), 0);
	}
	return (1);
}

void	print_action(t_philo *philo, int action)
{
	pthread_mutex_lock(&philo->args->print);
	if (action == THINK)
	printf("Philosopher %d is thinking\n", philo->id);
	else if (action == LEFT_FORK)
	printf("Philosopher %d took left fork %d\n", philo->id, philo->left);
	else if (action == RIGHT_FORK)
	printf("Philosopher %d took right fork %d\n", philo->id, philo->right);
	else if (action == EAT)
	{
		philo->last_meal = time_in_ms();
		pthread_mutex_lock(&philo->args->meal_mutex);
		(philo->meal_count)++;
		pthread_mutex_unlock(&philo->args->meal_mutex);
		printf("%ld ms Philosopher %d is eating meal count %d\n", philo->args->time_to_eat + time_in_ms(), philo->id, philo->meal_count);
	}
	else if (action == SLEEP)
		printf("%ld ms Philosopher %d is sleeping\n",philo->args->time_to_sleep + time_in_ms(), philo->id);
	pthread_mutex_unlock(&philo->args->print);
}

void	eat(t_philo *philo, int first_fork, int second_fork)
{
	pthread_mutex_lock(&philo->args->forks[first_fork]);
	if (philo->id % 2)
		print_action(philo, LEFT_FORK);
	else
		print_action(philo, RIGHT_FORK);
	pthread_mutex_lock(&philo->args->forks[second_fork]);
	if (philo->id % 2)
		print_action(philo, RIGHT_FORK);
	else
		print_action(philo, LEFT_FORK);
	print_action(philo, EAT);
	philo->last_meal = time_in_ms();
	usleep(philo->args->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->args->forks[first_fork]);
	pthread_mutex_unlock(&philo->args->forks[second_fork]);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int done;

	philo = (t_philo *)arg;
	while (check_died(philo))
	{
		done = philo->meal_count >= philo->args->must_eat_count;
		if(done)
			break;
		print_action(philo, THINK);
		if (philo->id % 2)
		{
			eat(philo, philo->left, philo->right);
			print_action(philo, SLEEP);
			usleep(philo->args->time_to_sleep * 1000);
		}
		else
		{
			print_action(philo, SLEEP);
			usleep(philo->args->time_to_sleep * 1000);
			eat(philo, philo->right, philo->left);
		}
	}
	return (NULL);
}

t_philo	*init_philo(t_args *args, t_philo *philo)
{
	int	i;

	philo = malloc(sizeof(t_philo) * args->nbr_philos);
	args->forks = malloc(sizeof(pthread_mutex_t) * args->nbr_philos);
	if (!philo || !args->forks)
		return (printf(ERR_ALLOC), exit(1), NULL);
	i = 0;
	if (pthread_mutex_init(&args->print, NULL))
		return (printf(ERR_ALLOC), NULL);
	while (i < args->nbr_philos)
	{
		philo[i].meal_count = 0;
		philo[i].args = args;
		philo[i].id = i;
		if (i == 0)
			philo[i].left = philo[i].args->nbr_philos - 1;
		else
			philo[i].left = i - 1;
		philo[i].right = i;
		if (pthread_mutex_init(&args->forks[i], NULL))
			return (printf(ERR_ALLOC), NULL);
		i++;
	}
	return (philo);
}

int	main(int argc, char *argv[])
{
	t_args	args;
	t_philo	*philo;
	int		i;

	pars_args(&args, argc, argv);
	philo = NULL;
	philo = init_philo(&args, philo);
	i = 0;
	while (i < args.nbr_philos)
	{
		pthread_create(&philo[i].tid, NULL, routine, &philo[i]);
		i++;
	}
	i = 0;
	while (i < args.nbr_philos)
		pthread_join(philo[i++].tid, NULL);
	return (0);
}
