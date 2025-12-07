#!/bin/bash

LOG_FILE="$1"

if [ ! -f "$LOG_FILE" ]; then
    echo "❌ Log file no encontrado: $LOG_FILE"
    exit 1
fi

echo "📊 Filósofos detectados: $(grep -Eo '^[0-9]+ [0-9]+' "$LOG_FILE" | awk '{print $2}' | sort -n | uniq | wc -l)"

EAT_COUNT=$(grep "is eating" "$LOG_FILE" | awk '{print $2}' | sort -n | uniq | wc -l)
echo "🍝 Filósofos que comieron al menos una vez: $EAT_COUNT"

DEATHS=$(grep "died" "$LOG_FILE" | wc -l)
echo "💀 Filósofos que murieron: $DEATHS"

if [ "$DEATHS" -eq 0 ]; then
	if [ "$EAT_COUNT" -ge 1 ]; then
		echo "✅ Resultado: Todos los filósofos comieron y ninguno murió."
	else
		echo "⚠️ Alerta: Ningún filósofo comió."
	fi
else
	echo "❌ Resultado: Algunos filósofos murieron."
fi

# Limpiar binarios y objetos
echo -e "\n${CYAN}🧹Limpiando archivos objeto y binarios...${NC}"
make -C philo fclean > /dev/null
make -C philo_bonus fclean > /dev/null