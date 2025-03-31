/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:50:52 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 17:22:54 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(int time_in_ms)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms)
		usleep(500);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_lock);
	if (!philo->data->someone_died)
		printf("%ld %d %s\n",
			get_time() - philo->data->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_lock);
}

void	drop_forks(t_philo *philo)
{
	int		id;
	int		left;
	int		right;

	id = philo->id - 1;
	left = id;
	right = (id + 1) % philo->data->num_philos;
	pthread_mutex_unlock(&philo->data->forks[left]);
	pthread_mutex_unlock(&philo->data->forks[right]);
}

int	start_simulation(t_data *data)
{
	pthread_t	monitor;
	int			i;

	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread,
				NULL, philosophers_life, &data->philos[i]) != 0)
			return (printf("Error: fallo al crear hilo.\n"), 1);
	}
	if (pthread_create(&monitor, NULL, death_monitor, data) != 0)
		return (printf("Error: fallo al crear hilo monitor.\n"), 1);
	pthread_join(monitor, NULL);
	i = -1;
	while (++i < data->num_philos)
		pthread_join(data->philos[i].thread, NULL);
	return (0);
}
