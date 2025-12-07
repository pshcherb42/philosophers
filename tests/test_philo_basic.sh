#!/bin/bash

echo "🔨 Compilando philo..."
make -C philo re

echo "🔨 Compilando philo_bonus..."
make -C philo_bonus re

# Crear carpeta para logs
mkdir -p logs

echo ""
echo "🧵 Testing PHILO (threads)"
echo "Test 1: ./philo/philo 1 300 200 200"
./philo/philo 1 300 200 200 > logs/philo_test1.txt

echo "Test 2: ./philo/philo 5 800 200 200 5"
./philo/philo 5 800 200 200 5 > logs/philo_test2.txt

echo "Test 3: ./philo/philo 5 800 200 200 3"
./philo/philo 5 800 200 200 3 > logs/philo_test3.txt

echo "Test 4 (error): ./philo/philo 0 800 200 200"
./philo/philo 0 800 200 200 > logs/philo_test4_error.txt

echo "🧪 Test 5 (zero time to die): ./philo/philo 5 0 200 200"
./philo/philo 5 0 200 200 > logs/philo_test5_zero_die.txt

echo "🧪 Test 6 (zero meals): ./philo/philo 5 800 200 200 0"
./philo/philo 5 800 200 200 0 > logs/philo_test6_zero_meals.txt

echo ""
echo "👥 Testing PHILO_BONUS (processes)"
echo "Test 1: ./philo_bonus/philo_bonus 1 300 200 200"
./philo_bonus/philo_bonus 1 300 200 200 > logs/bonus_test1.txt

echo "Test 2: ./philo_bonus/philo_bonus 5 600 150 150 5"
./philo_bonus/philo_bonus 5 600 150 150 5 > logs/bonus_test2.txt

echo "Test 3: ./philo_bonus/philo_bonus 5 800 200 200 4"
./philo_bonus/philo_bonus 5 800 200 200 4 > logs/bonus_test3.txt

echo "Test 4: ./philo_bonus/philo_bonus 5 200 200 200"
./philo_bonus/philo_bonus 5 200 200 200 > logs/bonus_test4_die.txt

echo "Test 5: ./philo_bonus/philo_bonus 100 800 200 200 1"
./philo_bonus/philo_bonus 100 800 200 200 1 > logs/bonus_test5_stress.txt

echo "Test 6: ./philo_bonus/philo_bonus 5 800 200 200 0"
./philo_bonus/philo_bonus 5 800 200 200 0 > logs/bonus_test6_zero_meals.txt

echo ""
echo "🧹 Limpiando archivos objeto y binarios..."
make -C philo fclean
make -C philo_bonus fclean

echo "✅ Tests completados. Revisa los resultados en el directorio ./logs/"