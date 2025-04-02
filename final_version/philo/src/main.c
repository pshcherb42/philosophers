/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:50:41 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 12:52:05 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (init_data(&data, argc, argv))
		return (1);
	init_philosophers(&data);
	if (start_simulation(&data))
		return (1);
	i = -1;
	while (++i < data.num_philos)
		pthread_mutex_destroy(&data.forks[i]);
	pthread_mutex_destroy(&data.print_lock);
	pthread_mutex_destroy(&data.death_lock);
	pthread_mutex_destroy(&data.full_lock);
	free(data.forks);
	free(data.philos);
}
