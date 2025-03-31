#include "../philo_bonus.h"
int     main(int argc, char **argv)
{
    t_data  data;

    if (init_data(&data, argc, argv))
        return (1);
    
    start_simulation(&data);

    sem_close(data.forks);
    sem_close(data.print_lock);
    sem_close(data.finish_lock);

    sem_unlink("/forks");
    sem_unlink("/print_lock");
    sem_unlink("/finish_lock");

    free(data.pids);
    return (0);
}