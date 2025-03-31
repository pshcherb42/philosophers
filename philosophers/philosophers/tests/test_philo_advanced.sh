#!/bin/bash

# === CONFIG ===
PHILO_DIR="./philo"
PHILO_BONUS_DIR="./philo_bonus"
PHILO_EXEC="$PHILO_DIR/philo"
PHILO_BONUS_EXEC="$PHILO_BONUS_DIR/philo_bonus"
LOG_DIR="./test_logs"
mkdir -p $LOG_DIR

# === FUNCIONES DE COMPILACION Y LIMPIEZA ===
compile_projects() {
	echo "🔨 Compilando philo..."
	make -C $PHILO_DIR >/dev/null || { echo "❌ Error compilando philo"; exit 1; }
	echo "🔨 Compilando philo_bonus..."
	make -C $PHILO_BONUS_DIR >/dev/null || { echo "❌ Error compilando philo_bonus"; exit 1; }
}

cleanup() {
	echo "🧹 Limpiando archivos objeto y binarios..."
	make -C $PHILO_DIR fclean >/dev/null
	make -C $PHILO_BONUS_DIR fclean >/dev/null
}

run_test() {
	echo "Test $1: $2"
	$2 > "$LOG_DIR/$1.out"
}

# === TESTS DE ERRORES ===
run_error_tests() {
	echo ""
	echo "🚨 Testing ERRORES"
	run_test "E1" "$PHILO_EXEC 0 800 200 200"
	run_test "E2" "$PHILO_EXEC 5 0 200 200"
	run_test "E3" "$PHILO_BONUS_EXEC 5 800 200 200 0"

	# Simulando error de sem_open con nombre ya tomado
	sem_name="/forks_test"
	sem_open_code="sem_unlink(\"$sem_name\"); sem_t *forks = sem_open(\"$sem_name\", 0); if (forks == SEM_FAILED) { printf(\"Error: sem_open failed\\n\"); exit(1); }"
	echo -e "#include <stdio.h>\n#include <stdlib.h>\n#include <semaphore.h>\n#include <fcntl.h>\n
int main() { $sem_open_code return 0; }" > sem_test.c
	gcc sem_test.c -o sem_test && ./sem_test > "$LOG_DIR/E4.out"
	rm sem_test sem_test.c

	# No podemos simular malloc o pthread_create fallando sin cambiar código
	echo "Test E5: Simulación de malloc fallido (requiere modificación del código)" > "$LOG_DIR/E5.out"
	echo "Test E6: Simulación de pthread_create fallido (requiere modificación del código)" > "$LOG_DIR/E6.out"
}

# === MAIN ===
compile_projects
run_error_tests
cleanup
echo "✅ Tests completados. Resultados en carpeta $LOG_DIR"
