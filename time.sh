#!/bin/bash

LINUX_LS="ls"

# Number of iterations
ITERATIONS=100

run_ls_and_average_time() {
  local grep_command="$1"

  total_time=0
  for ((i = 1; i <= ITERATIONS; i++)); do
    start_time=$(date +%s.%N)

    eval "$grep_command" >/dev/null

    end_time=$(date +%s.%N)
    execution_time=$(echo "$end_time - $start_time" | bc)
    total_time=$(echo "$total_time + $execution_time" | bc)
  done

  average_time=$(echo "scale=4; $total_time / $ITERATIONS" | bc)
  printf "\tAverage execution time: %.4f seconds\n" "$average_time"
}

echo "Testing Linux ls..."
run_ls_and_average_time "$LINUX_LS"

echo "Testing my ls..."
make build >/dev/null
run_ls_and_average_time "./ls"
make clean
