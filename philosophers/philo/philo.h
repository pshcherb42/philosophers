#ifndef  PHILO_H
# define PHILO_H 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_philo
{
    int	id;
	int meals_eaten;
	long last_meal_time;
	pthread_t	thread;
	struct s_data *data;
} t_philo;

typedef struct s_data
{
	int num_philos;
	int time_to_die;
	int	time_to_eat;
	int time_to_sleep;
	int must_eat;
	long start_time;
	int	someone_died;
	int	full_philos; // ✅ Cuantos filosofos ya comieron lo sufieciente
	pthread_mutex_t	full_lock;
	pthread_mutex_t death_lock;
	pthread_mutex_t *forks;
	pthread_mutex_t print_lock;
	t_philo *philos;
} t_data;

int 	init_data(t_data *data, int argc, char **argv);
void    *death_monitor(void *arg);
int		someone_died(t_data *data);
void    eat(t_philo *philo);
void    drop_forks(t_philo *philo);
void    take_forks(t_philo *philo);
void    init_philosophers(t_data *data);
void    *philosophers_life(void *arg);
int		start_simulation(t_data *data);
long 	get_time();
void 	ft_usleep(int time_in_ms);
void 	print_status(t_philo *philo, char *status);

#endif