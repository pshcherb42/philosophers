#!/bin/bash

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
CYAN='\033[0;36m'
NC='\033[0m'

# Crear carpeta de logs
mkdir -p test_logs

echo -e "${CYAN}🔨Compilando philo...${NC}"
make -C philo > /dev/null

echo -e "${CYAN}🔨Compilando philo_bonus...${NC}"
make -C philo_bonus > /dev/null

echo -e "\n🚨${RED}Testing ERRORES${NC}"

# Test E1: Numero de filósofos invalido
echo -e "${CYAN}Test E1: ./philo/philo 0 800 200 200${NC}"
./philo/philo 0 800 200 200 > test_logs/philo_error1.txt 2>&1

# Test E2: Tiempo de vida inválido
echo -e "${CYAN}Test E2: ./philo/philo 5 0 200 200${NC}"
./philo/philo 5 0 200 200 > test_logs/philo_error2.txt 2>&1

# Test E3: Valor 0 en meals (deberia salir al instante)
echo -e "${CYAN}Test E3: ./philo/philo 5 800 200 200 0${NC}"
./philo/philo 5 800 200 200 0 > test_logs/philo_bonus_zero_meals.txt 2>&1

# Test E4: Simulacion fallo de semaforo 
echo -e "${CYAN}Test E4: Simulacion fallo de sem_open${NC}"
cat > sem_test.c <<EOF
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

int main()
{
	sem_unlink("/forks_test");
	sem_t *forks = sem_open("/forks_test", 0);
	if (forks == SEM_FAILED)
	{
		printf("Error: sem_open failed\n");
		exit(1);
	}
	sem_close(forks);
	return (0);
}
EOF

# Compilar y guardar test de sem_open
gcc sem_test.c -o sem_test 
./sem_test > test_logs/sem_test_output.txt 2>&1
rm -f sem_test sem_test.c

# Limpiar binarios y objetos
echo -e "\n${CYAN}🧹Limpiando archivos objeto y binarios...${NC}"
make -C philo fclean > /dev/null
make -C philo_bonus fclean > /dev/null

echo -e "${GREEN}✅ Test completados. Resultados en carpeta ./test_logs${NC}"