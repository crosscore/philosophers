#!/bin/bash
success_count=0
total_runs=100
for i in $(seq 1 $total_runs)
do
	result=$(./philo 3 4 1 1 100 | grep "philosophers had at least")
	if [ -n "$result" ]; then
		a=$(echo "$result" | cut -d'/' -f1)
		b=$(echo "$result" | cut -d'/' -f2 | cut -d' ' -f1)
		if [ "$a" == "$b" ]; then
			success_count=$((success_count + 1))
		fi
	fi
	printf "\rCompleted trials: [%03d/%03d]" "$i" "$total_runs"
	printf "\nSuccess count: [%03d/%03d]" "$success_count" "$total_runs"
	printf "\033[A" # Move cursor up one line
done
printf "\n"
success_rate=$((success_count * 100 / total_runs))
printf "Final success count: [%03d/%03d]" "$success_count" "$total_runs"
printf "\n"
echo "Success rate: $success_rate%"