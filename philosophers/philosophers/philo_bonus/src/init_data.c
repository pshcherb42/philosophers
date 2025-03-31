/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:04:26 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 17:28:19 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

static int	parse_arguments(t_data *data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: argumentos incorrectos\n");
		return (1);
	}
	if (argc == 6)
		data->must_eat = atoi(argv[5]);
	else
		data->must_eat = -1;
	data->num_philos = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	data->start_time = get_time();
	data->pids = malloc(sizeof(pid_t) * data->num_philos);
	if (!data->pids)
	{
		printf("Error: malloc\n");
		return (1);
	}
	return (0);
}

static int	open_semaphores(t_data *data)
{
	sem_unlink("/forks");
	sem_unlink("/print_lock");
	sem_unlink("/finish_lock");
	data->forks = sem_open("/forks", O_CREAT, 0644, data->num_philos);
	data->print_lock = sem_open("/print_lock", O_CREAT, 0644, 1);
	data->finish_lock = sem_open("/finish_lock", O_CREAT, 0644, 0);
	if (data->forks == SEM_FAILED || data->print_lock == SEM_FAILED
		|| data->finish_lock == SEM_FAILED)
	{
		printf("Error: sem_open\n");
		return (1);
	}
	return (0);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (parse_arguments(data, argc, argv))
		return (1);
	if (open_semaphores(data))
		return (1);
	return (0);
}
