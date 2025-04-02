/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pshcherb <pshcherb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 15:06:28 by pshcherb          #+#    #+#             */
/*   Updated: 2025/04/02 12:12:14 by pshcherb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

static void	create_processes(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
	{
		data->pids[i] = fork();
		if (data->pids[i] == 0)
		{
			data->start_time = get_time();
			rutina_filosofo(data, i + 1);
		}
	}
}

static void	wait_for_termination(t_data *data, int *should_kill)
{
	int	status;
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		sem_wait(data->finish_lock);
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
			*should_kill = 1;
		i++;
	}
}

static void	kill_all_processes(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
		kill(data->pids[i], SIGKILL);
}

void	start_simulation(t_data *data)
{
	int	should_kill;

	should_kill = 0;
	if (data->must_eat == 0)
	{
		printf("Todos han comido 0 veces. Fin de la simulacion.\n");
		return ;
	}
	create_processes(data);
	wait_for_termination(data, &should_kill);
	if (should_kill)
		kill_all_processes(data);
}
