#!/bin/bash

PHILO_BIN="./philo_bonus/philo_bonus"
DURATION=60
N_PHILOS=200
LOG_DIR="stress_logs"
mkdir -p "$LOG_DIR"
LOG_FILE="$LOG_DIR/stress_philo_bonus_$(date +%s).log"

echo "🚀 Prueba de estrés con $N_PHILOS filósofos durante $DURATION segundos..."

# Ejecutamos en segundo plano y guardamos el PID
$PHILO_BIN $N_PHILOS 800 200 200 1 > "$LOG_FILE" &
PHILO_PID=$!

# Esperamos el tiempo deseado
sleep $DURATION

# Verificamos si el proceso sigue vivo
if ps -p $PHILO_PID > /dev/null; then
    echo "⏱️ Tiempo agotado. Terminando el proceso..."
    kill $PHILO_PID
    wait $PHILO_PID 2>/dev/null
else
    echo "⚠️ Terminó antes del tiempo previsto"
fi 

echo "📄 Logs guardados en $LOG_FILE"

echo -e "\n🔍 Analizando log generado..."
bash ./tests/analyze_stress_log.sh "$LOG_FILE"