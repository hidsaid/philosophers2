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

static int	ft_atoi(const char *s)
{
	int		sign;
	long	res;
	int		i;

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
		if (res * sign > INT_MAX)
			return (printf(ERR_NOT_INT), exit(1), 1);
	}
	return ((int)res * sign);
}



void	pars_args(t_args *args, int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc <= 4 || argc > 6)
		return (printf(ERR_ARG_COUNT), exit(1));
	args->nbr_philos = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	(argc == 6) && (args->must_eat_count = ft_atoi(argv[5]));
	while (i < argc)
	{
		if (!check_is_number(argv[i++]))
			return (printf(ERR_NON_NUMERIC), exit(1));
	}
	if (args->nbr_philos < 1 || args->time_to_die <= 0
		|| args->time_to_eat <= 0 || args->time_to_sleep <= 0 || (argc == 6
			&& args->must_eat_count <= 0))
		return (printf(ERR_INVALID_VALUES), exit(1));
}
