/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:39:52 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 16:45:50 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	validate_arguments(t_data *data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: numero incorrecto de argumentos\n");
		return (1);
	}
	data->num_philos = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		data->must_eat = atoi(argv[5]);
	else
		data->must_eat = -1;
	if (data->num_philos < 1 || data->time_to_die < 1
		|| data->time_to_eat < 1 || data->time_to_sleep < 1)
	{
		printf("Error: argumentos inválidos.\n");
		return (1);
	}
	return (0);
}

static int	init_mutexes(t_data *data)
{
	int	i;

	pthread_mutex_init(&data->death_lock, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	pthread_mutex_init(&data->full_lock, NULL);
	i = -1;
	while (++i < data->num_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (validate_arguments(data, argc, argv))
		return (1);
	data->someone_died = 0;
	data->full_philos = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->forks || !data->philos)
	{
		printf("Error: fallo malloc.\n");
		return (1);
	}
	init_mutexes(data);
	data->start_time = get_time();
	return (0);
}

void	init_philosophers(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_time();
		data->philos[i].data = data;
	}
}
