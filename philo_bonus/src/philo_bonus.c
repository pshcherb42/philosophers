/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:51:44 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 15:07:14 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	*monitor_de_muerte(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		ft_usleep(1);
		if ((get_time() - philo->last_meal) > philo->data->time_to_die)
		{
			sem_wait(philo->data->print_lock);
			printf("%ld %d died\n",
				get_time() - philo->data->start_time, philo->id);
			sem_post(philo->data->print_lock);
			sem_post(philo->data->finish_lock);
			free(philo);
			exit(1);
		}
	}
	return (NULL);
}

void	rutina_accion(t_philo *philo)
{
	while (1)
	{
		print_status(philo, "is thinking");
		sem_wait(philo->data->forks);
		print_status(philo, "has taken a fork");
		sem_wait(philo->data->forks);
		print_status(philo, "has taken a fork");
		print_status(philo, "is eating");
		philo->last_meal = get_time();
		ft_usleep(philo->data->time_to_eat);
		philo->meals_eaten++;
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
		if (philo->data->must_eat > 0
			&& philo->meals_eaten >= philo->data->must_eat)
		{
			sem_post(philo->data->finish_lock);
			free(philo);
			exit(0);
		}
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
	}
}

void	rutina_filosofo(t_data *data, int id)
{
	t_philo		*philo;
	pthread_t	monitor;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		exit(1);
	philo->id = id;
	philo->data = data;
	philo->meals_eaten = 0;
	philo->last_meal = get_time();
	pthread_create(&monitor, NULL, monitor_de_muerte, philo);
	pthread_detach(monitor);
	rutina_accion(philo);
}
