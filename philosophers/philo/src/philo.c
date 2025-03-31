#include "../philo.h"

long    get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void    ft_usleep(int time_in_ms)
{
    long    start;
    
    start = get_time();
    while ((get_time() - start) < time_in_ms)
        usleep(500);
}

int init_data(t_data *data, int argc, char **argv)
{
    if (argc < 5 || argc > 6)
        return (printf("Error: numero incorecto de argumentos.\n"), 1);
    data->num_philos = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    data->someone_died = 0;
    pthread_mutex_init(&data->death_lock, NULL);
    data->full_philos = 0;
    pthread_mutex_init(&data->full_lock, NULL);
    data->must_eat = (argc == 6) ? atoi(argv[5]) : -1;

    if (data->num_philos < 1 || data->time_to_die < 1 || data->time_to_eat < 1 
        || data->time_to_sleep < 1)
        return (printf("Error: argumentos invalidos.\n"), 1);
    
    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
    data->philos = malloc(sizeof(t_philo) * data->num_philos);
    if (!data->forks || !data->philos)
        return (printf("Error: fallo maloc.\n"), 1);
    int i;
    i = -1;
    while (++i < data->num_philos)
        pthread_mutex_init(&data->forks[i], NULL);
    pthread_mutex_init(&data->print_lock, NULL);
    pthread_mutex_init(&data->death_lock, NULL);
    data->start_time = get_time();
    return (0);
}

void    init_philosophers(t_data *data)
{
    int i;

    i = -1;
    while (++i < data->num_philos)
    {
        data->philos[i].id = i + 1;
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal_time = get_time();
        data->philos[i].data = data;
    }
}

void    take_forks(t_philo *philo)
{
    int id = philo->id - 1; //porque -1?
    int left = id;
    int right = (id + 1) % philo->data->num_philos;

    // Evitar deadlock: los pares cogen primero derecha, luego izquerda
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

void    drop_forks(t_philo *philo)
{
    int id;
    int left;
    int right;

    id = philo->id - 1;
    left = id;
    right = (id + 1) % philo->data->num_philos;
    pthread_mutex_unlock(&philo->data->forks[left]);
    pthread_mutex_unlock(&philo->data->forks[right]);
}    

void    eat(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->death_lock);
    philo->last_meal_time = get_time();
    pthread_mutex_unlock(&philo->data->death_lock);

    print_status(philo, "is eating");
    ft_usleep(philo->data->time_to_eat);
    philo->meals_eaten++;

    // ✅ Comprobamos si ya comio suficiente
    if (philo->data->must_eat > 0 && philo->meals_eaten == philo->data->must_eat)
    {
        pthread_mutex_lock(&philo->data->full_lock);
        philo->data->full_philos++;
        pthread_mutex_unlock(&philo->data->full_lock);
    }
}

void    *philosophers_life(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    if (philo->id % 2 == 0)
        usleep(1000);
    if (philo->data->num_philos == 1)
    {
        pthread_mutex_lock(&philo->data->forks[0]);
        print_status(philo, "has taken a fork");
        
        ft_usleep(philo->data->time_to_die);
            
        pthread_mutex_lock(&philo->data->death_lock);
        if(!philo->data->someone_died)
        {
            philo->data->someone_died = 1;
            pthread_mutex_unlock(&philo->data->death_lock);
        
            pthread_mutex_lock(&philo->data->print_lock);
            printf("%ld %d died\n", get_time() - philo->data->start_time, philo->id);
            pthread_mutex_unlock(&philo->data->print_lock);
        }
        else
            pthread_mutex_unlock(&philo->data->death_lock);
        
        pthread_mutex_unlock(&philo->data->forks[0]);
        return NULL;
    }
    while (1)
    {
        pthread_mutex_lock(&philo->data->death_lock);
        if (philo->data->someone_died)
        {
            pthread_mutex_unlock(&philo->data->death_lock);
            break;
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

int start_simulation(t_data *data)
{
    pthread_t monitor;
    int i;

    i = -1;
    while (++i < data->num_philos)
    {
        if (pthread_create(&data->philos[i].thread, NULL, philosophers_life, &data->philos[i]) != 0)
            return (printf("Error: fallo al crear hilo.\n"), 1);
    }

    // 👉 Lanzar el monitor aqui, despues de crear los filosofos
    if (pthread_create(&monitor, NULL, death_monitor, data) != 0)
        return (printf("Error: fallo al crear hilo monitor.\n"), 1);

    pthread_join(monitor, NULL);

    // ✅ Una vez el monitor detecte la muerte, podemos esperar que todos terminen
    i = -1;
    while (++i < data->num_philos)
        pthread_join(data->philos[i].thread, NULL);
    
    return (0);
}

void    print_status(t_philo *philo, char *status)
{
    pthread_mutex_lock(&philo->data->print_lock);
    printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id, status);
    pthread_mutex_unlock(&philo->data->print_lock);
}

void    *death_monitor(void *arg)
{
    t_data *data;
    data = (t_data *)arg;
    while (1)
    {
        for (int i = 0; i < data->num_philos; i++)
        {
            long now = get_time();
            pthread_mutex_lock(&data->death_lock);
            long time_since_last_meal = now - data->philos[i].last_meal_time;
            pthread_mutex_unlock(&data->death_lock);

            if (time_since_last_meal > data->time_to_die)
            {
                pthread_mutex_lock(&data->death_lock);
                if (data->someone_died)
                {
                    pthread_mutex_unlock(&data->death_lock);
                    return (NULL);
                }
                data->someone_died = 1;
                pthread_mutex_unlock(&data->death_lock);

                pthread_mutex_lock(&data->print_lock);
                printf("%ld %d died\n", now - data->start_time, data->philos[i].id);
                pthread_mutex_unlock(&data->print_lock);

                return (NULL); // salir del hilo monitor
            }
        }
        // ✅ Verificar si todos comieron suficiente
        if (data->must_eat > 0)
        {
            pthread_mutex_lock(&data->full_lock);
            if (data->full_philos == data->num_philos)
            {
                pthread_mutex_lock(&data->death_lock);
                data->someone_died = 1; // ⚠️ usamos este flag como "stop", aunque no haya muerto nadie
                pthread_mutex_unlock(&data->death_lock);

                pthread_mutex_unlock(&data->full_lock);
                return (NULL);
            }
            pthread_mutex_unlock(&data->full_lock);
        }
        usleep(1000); // evitar sobrecargar la CPU
    }
    return (NULL);
}