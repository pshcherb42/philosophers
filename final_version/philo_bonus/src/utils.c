/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:51:51 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 15:09:52 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_usleep(int ms)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < ms)
		usleep(500);
}

void	print_status(t_philo *philo, char *status)
{
	sem_wait(philo->data->print_lock);
	printf("%ld %d %s\n",
		get_time() - philo->data->start_time, philo->id, status);
	sem_post(philo->data->print_lock);
}
