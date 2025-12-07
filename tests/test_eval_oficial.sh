#!/bin/bash

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

PHILO="./philo/philo"
PHILO_BONUS="./philo_bonus/philo_bonus"

run_test()
{
    echo -e "\n$1"
    $2 > tmp.log &
    PID=$!
    sleep $3
    kill $PID 2>/dev/null
    cat tmp.log
    rm -f tmp.log
}

echo -e "${GREEN}🔨Compilando philo y philo_bonus...${NC}"
make -C philo > /dev/null && make -C philo_bonus > /dev/null

# === PHILO (MANDATORY) ===
echo -e "\n${GREEN}📘 TEST PHILO (MANDATORY PART)${NC}"

run_test "🧪 Test 1 (1 300 200 200) -> debe morir" "$PHILO 1 300 200 200" 3
run_test "🧪 Test 2 (5 800 200 200) -> nadie debe morir" "$PHILO 5 800 200 200" 3
run_test "🧪 Test 3 (5 800 200 200 7) -> termina cuando todos comen 7 veces" "$PHILO 5 800 200 200 7" 5
run_test "🧪 Test 4 (4 410 200 200) -> nadie debe morir" "$PHILO 4 410 200 200" 4
run_test "🧪 Test 5 (4 3 10 200 100) -> debe morir" "$PHILO 4 3 10 200 100" 2

# === PHILO BONUS ===
echo -e "\n${GREEN}📗 TESTS PHILO_BONUS${NC}"

run_test "🧪 Bonus 1 (5 800 200 200) -> nadie debe morir" "$PHILO_BONUS 5 800 200 200" 3
run_test "🧪 Bonus 2 (5 800 200 200 7) -> termina cuando todos comen 7 veces" "$PHILO_BONUS 5 800 200 200 7" 5
run_test "🧪 Bonus 3 (4 410 200 200) -> nadie debe morir" "$PHILO_BONUS 4 410 200 200" 4
run_test "🧪 Bonus 4 (4 3 10 200 100) -> debe morir" "$PHILO_BONUS 4 3 10 200 100" 2

# === CLEAN
make -C philo fclean > /dev/null && make -C philo_bonus fclean > /dev/null