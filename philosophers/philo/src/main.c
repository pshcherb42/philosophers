#include "../philo.h"
int main(int argc, char **argv)
{
    t_data  data;
    int     i;

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