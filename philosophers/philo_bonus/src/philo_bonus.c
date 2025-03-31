#include "../philo_bonus.h"

int     init_data(t_data *data, int argc, char **argv)
{
    if (argc < 5 || argc > 6)
        return (printf("Error: argumentos incorrectos\n"), 1);

    data->num_philos = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    data->must_eat = (argc == 6) ? atoi(argv[5]) : -1;
    data->start_time = get_time();
    data->pids = malloc(sizeof(pid_t) * data->num_philos);
    if (!data->pids)
        return (printf("Error: malloc\n"), 1);
    
    // Crear semaforos
    sem_unlink("/forks"); // Borra el semaforo viejo si existe
    sem_unlink("/print_lock");
    sem_unlink("/finish_lock");
    data->forks = sem_open("/forks", O_CREAT, 0644, data->num_philos);
    data->print_lock = sem_open("/print_lock", O_CREAT, 0644, 1);
    data->finish_lock = sem_open("/finish_lock", O_CREAT, 0644, 0);

    if (data->forks == SEM_FAILED || data->print_lock == SEM_FAILED || data->finish_lock == SEM_FAILED)
        return (printf("Error: sem_open\n"), 1);
    
    return (0);
}

void    *monitor_de_muerte(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        ft_usleep(1); // no bloquea CPU, pero da respiro
        if ((get_time() - philo->last_meal) > philo->data->time_to_die)
        {
            sem_wait(philo->data->print_lock);
            printf("%ld %d died\n", get_time() - philo->data->start_time, philo->id);
            sem_post(philo->data->print_lock);
            
            sem_post(philo->data->finish_lock);
            free(philo); // ✅ liberar antes de morir
            exit(1); // muerte
        }
    }
    return (NULL);
}

void    rutina_filosofo(t_data *data, int id)
{
    t_philo *philo; // porque lo creamos otra vez? no lo tenemos declarado en .h?
    pthread_t monitor;

    philo = malloc(sizeof(t_philo));
    if (!philo)
        exit(1);
    
    // 🔄 Inicializamos filosofo
    philo->id = id; // porque ahora no es id - 1?
    philo->data = data;
    philo->meals_eaten = 0;
    philo->last_meal = get_time(); // porque?

    // 🧠 Lanzamos hilo de monitor
    pthread_create(&monitor, NULL, monitor_de_muerte, philo);
    pthread_detach(monitor); // No esperamos que termine

    while (1)
    {
        // pensar
        print_status(philo, "is thinking");

        // Tomar dos tenedores (esperar dos veces en forks)
        sem_wait(data->forks);
        print_status(philo, "has taken a fork");
        sem_wait(data->forks);
        print_status(philo, "has taken a fork");

        // Comer
        print_status(philo, "is eating");
        philo->last_meal = get_time(); // es para ver cuanto tiempo paso sin comer?
        ft_usleep(data->time_to_eat);
        philo->meals_eaten++;

        // Soltar los tenedores
        sem_post(data->forks); // que hace sem_post?
        sem_post(data->forks);

        // Verificar si debe salir
        if (data->must_eat > 0 && philo->meals_eaten >= data->must_eat)
        {
            sem_post(data->finish_lock); // Avisa al padre; que es finish_lock?
            free(philo); // ✅ limpieza
            exit(0);
        }

        // Dormir
        print_status(philo, "is sleeping");
        ft_usleep(data->time_to_sleep);
    }
}

void    start_simulation(t_data *data)
{
    int     i;

    i = -1;
    while (++i < data->num_philos)
    {
        data->pids[i] = fork();
        if (data->pids[i] == 0)
        {
            data->start_time = get_time(); // 🔁 sincroniza justo antes de forking
            // En proceso hijo -> ejecuta rutina
            rutina_filosofo(data, i + 1); // i + 1 porque en arrays i va de 0 a N - 1
        }
    }

    int     status;
    for (int j = 0; j < data->num_philos; j++)
    {
        sem_wait(data->finish_lock); // espera a cada filosofo

        // ✅ Checamos si algun proceso murio (exit(1))
        waitpid(-1, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
        {
            // alguin murió ▻ matar a todos
            for (int k = 0; k < data->num_philos; k++)
                kill(data->pids[k], SIGKILL);
            break ;
        }
    }
}