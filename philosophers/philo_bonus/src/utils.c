#include "../philo_bonus.h"

long    get_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL); // que es tv y gettimeofday?
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000); // sec * 1000 - segundos a milisegundos, usec / 1000 - miscrosegundos a milisegundos
    // segundos desde el año 1970?? y microsegunods desde ahora? porque los sumamos?
}

void    ft_usleep(int ms) // espera exactamente los ms milisegundos, sin sobrecargar CPU; que espera? para que esperar?
{
    long    start;

    start = get_time();
    while ((get_time() - start) < ms)
        usleep(500); // duerme 0.5 milisegundos
}

void    print_status(t_philo *philo, char *status)
{
    sem_wait(philo->data->print_lock); // que es print_lock y porque esperar?
    printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id, status); // donde esta status, que es ld? porque hacemos get_time - start_time?
    sem_post(philo->data->print_lock); // esto es para imprimirlo?
}