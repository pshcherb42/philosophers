/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:54:17 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 16:49:23 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	take_forks(t_philo *philo)
{
	int	id;
	int	left;
	int	right;

	id = philo->id - 1;
	left = id;
	right = (id + 1) % philo->data->num_philos;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[right]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[left]);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[left]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[right]);
		print_status(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->death_lock);
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	philo->meals_eaten++;
	if (philo->data->must_eat > 0
		&& philo->meals_eaten == philo->data->must_eat)
	{
		pthread_mutex_lock(&philo->data->full_lock);
		philo->data->full_philos++;
		pthread_mutex_unlock(&philo->data->full_lock);
	}
}

static void	*handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[0]);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die);
	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->someone_died)
	{
		philo->data->someone_died = 1;
		pthread_mutex_unlock(&philo->data->death_lock);
		pthread_mutex_lock(&philo->data->print_lock);
		printf("%ld %d died\n",
			get_time() - philo->data->start_time, philo->id);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
	else
		pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(&philo->data->forks[0]);
	return (NULL);
}

void	*philosophers_life(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (philo->data->num_philos == 1)
		return (handle_one_philo(philo));
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		if (philo->data->someone_died)
		{
			pthread_mutex_unlock(&philo->data->death_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_lock);
		print_status(philo, "is thinking");
		take_forks(philo);
		eat(philo);
		drop_forks(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
	}
	return (NULL);
}
