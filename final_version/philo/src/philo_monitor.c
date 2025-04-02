/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:55:24 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 17:10:30 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	check_death(t_data *data, int i, long now)
{
	long	time_since_last_meal;

	pthread_mutex_lock(&data->death_lock);
	time_since_last_meal = now - data->philos[i].last_meal_time;
	pthread_mutex_unlock(&data->death_lock);
	if (time_since_last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->death_lock);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->death_lock);
			return (1);
		}
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_lock);
		pthread_mutex_lock(&data->print_lock);
		printf("%ld %d died\n",
			now - data->start_time, data->philos[i].id);
		pthread_mutex_unlock(&data->print_lock);
		return (1);
	}
	return (0);
}

static int	check_all_full(t_data *data)
{
	if (data->must_eat > 0)
	{
		pthread_mutex_lock(&data->full_lock);
		if (data->full_philos == data->num_philos)
		{
			pthread_mutex_lock(&data->death_lock);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_lock);
			pthread_mutex_unlock(&data->full_lock);
			return (1);
		}
		pthread_mutex_unlock(&data->full_lock);
	}
	return (0);
}

void	*death_monitor(void *arg)
{
	t_data	*data;
	int		i;
	long	now;

	data = (t_data *)arg;
	while (1)
	{
		i = -1;
		while (++i < data->num_philos)
		{
			now = get_time();
			if (check_death(data, i, now))
				return (NULL);
		}
		if (check_all_full(data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
