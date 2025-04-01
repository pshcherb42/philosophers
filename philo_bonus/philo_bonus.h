/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:51:24 by pshcherb          #+#    #+#             */
/*   Updated: 2025/03/31 17:28:05 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <semaphore.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>

typedef struct s_data
{
	int		num_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		must_eat;
	long	start_time;
	pid_t	*pids;
	sem_t	*forks;
	sem_t	*print_lock;
	sem_t	*finish_lock;
}	t_data;

typedef struct s_philo
{
	int		id;
	int		meals_eaten;
	long	last_meal;
	t_data	*data;
}	t_philo;
long	get_time(void);
void	ft_usleep(int ms);
void	print_status(t_philo *philo, char *status);
int		init_data(t_data *data, int argc, char **argv);
void	start_simulation(t_data *data);
void	rutina_filosofo(t_data *data, int id);

#endif
