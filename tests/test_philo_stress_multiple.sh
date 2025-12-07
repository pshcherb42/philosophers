#!/bin/bash

RUNS=3
SLEEP_BETWEEN=5
PHILO_BIN="./philo_bonus/philo_bonus"
DURATION=60
N_PHILOS=150
LOG_DIR="stress_logs"
mkdir -p "$LOG_DIR"

echo "Ejecutando $RUNS pruebas de estres con $N_PHILOS filosofos..."

total_detected=0
total_ate=0
total_died=0

for i in $(seq 1 $RUNS); do
	echo ""
	echo "Prueba $i de $RUNS"

	LOG_FILE="$LOG_DIR/stress_philo_bonus_run${i}_$(date +%s).log"

	echo "Compilando philo_bonus..."
	make -C philo_bonus > /dev/null

	if [ ! -x "$PHILO_BIN" ]; then
	    echo "❌ Error: Binario $PHILO_BIN no encontrado o no ejecutable"
	    exit 1
	fi

	echo "Iniciando simulacion durante $DURATION segundos..."
	$PHILO_BIN $N_PHILOS 800 200 200 1 > "$LOG_FILE" &
	PHILO_PID=$!

	sleep $DURATION

	if kill -0 "$PHILO_PID" 2>/dev/null; then
		echo "Tiempo agotado. Terminando el proceso..."
		kill "$PHILO_PID"
		wait "$PHILO_PID" 2>/dev/null
	else
		echo "El proceso termino por si solo antes del tiempo limite."
	fi

	echo "Resultado guardado en $LOG_FILE"
	echo "Analizando log..."

	detected=$(grep "Filosofos detectados" "$LOG_FILE" | awk '{print $NF}')
	ate=$(grep "comieron al menos una vez" "$LOG_FILE" | awk '{print $NF}')
	died=$(grep "murieron" "$LOG_FILE" | awk '{print $NF}')

	# Default values if empty
	detected=${detected:-0}
	ate=${ate:-0}
	died=${died:-0}

	echo "Resumen Prueba $i: Detectados=$detected, Comieron=$ate, Murieron=$died"

	total_detected=$((total_detected + detected))
	total_ate=$((total_ate + ate))
	total_died=$((total_died + died))

	echo -e "\n Esperando $SLEEP_BETWEEN segundos antes de la siguiente prueba..."
	sleep $SLEEP_BETWEEN
done

echo ""
echo "=== RESUMEN FINAL DE LAS $RUNS PRUEBAS ==="
echo "Total filosofos detectados: $total_detected"
echo "Total que comieron al menos una vez: $total_ate"
echo "Total que murieron: $total_died"

echo ""
echo "Revisa todos los logs en $LOG_DIR"

