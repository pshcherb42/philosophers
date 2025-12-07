#!/bin/bash

PHILO_BIN="./philo_bonus/philo_bonus"
DURATION=60
N_PHILOS=200
LOG_DIR="stress_logs"
mkdir -p "$LOG_DIR"
LOG_FILE="$LOG_DIR/stress_philo_bonus_$(date +%s).log"

echo "🔨 Compilando philo_bonus..."
make -C philo_bonus re > /dev/null

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
    echo "El proceso finalizo por su cuenta antes de los $DURATION segundos"
    echo "Esto puede indicar que todos los filosofos comieron segun el objetivo (must_eat)."
fi 

# Limpiar binarios y objetos
echo -e "\n${CYAN}🧹Limpiando archivos objeto y binarios...${NC}"
make -C philo fclean > /dev/null
make -C philo_bonus fclean > /dev/null

echo "📄 Logs guardados en $LOG_FILE"

echo -e "\n🔍 Analizando log generado..."
bash ./tests/analyze_stress_log.sh "$LOG_FILE"