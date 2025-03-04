#!/bin/bash

OUTPUT_DIR="fuzzing_results"
NUM_TESTS=${1:-100}
MAX_LENGTH=1000
OPAL_EXECUTABLE="bin/opal"
LOG_FILE="$OUTPUT_DIR/fuzzing_log.txt"
REPORT_FILE="$OUTPUT_DIR/report.md"

mkdir -p $OUTPUT_DIR
rm -f $OUTPUT_DIR/error_*.txt
echo "=== Opal Fuzzing Log ===" > $LOG_FILE

function log_message() {
    local message=$1
    echo "$message" | tee -a $LOG_FILE
}

function generate_fuzz_sample() {
    local length=$1
    local sample=""
    local chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:'\",.<>?/"
    for ((i=0; i<$length; i++)); do
        sample+="${chars:RANDOM%${#chars}:1}"
    done
    echo "$sample"
}

function generate_random_input() {
    local choice=$((RANDOM % 5))
    case $choice in
        0) generate_fuzz_sample $MAX_LENGTH ;;
        1) echo "\"$(generate_fuzz_sample $((MAX_LENGTH / 2)))\"" ;;
        2) echo $((RANDOM)) ;;
        3) echo $((RANDOM / 1000)) ;;
        4) echo "/* $(generate_fuzz_sample $((MAX_LENGTH / 2))) */" ;;
    esac
}

success_count=0
fail_count=0
total_memory=0
max_memory=0
start_time=$(date +%s)

for ((i=1; i<=$NUM_TESTS; i++)); do
    sample=$(generate_random_input)
    echo "$sample" > temp_test.op
    
    memory_info=$(/usr/bin/time -f "%M" $OPAL_EXECUTABLE temp_test.op 2>&1)
    exit_code=$?
    memory_kb=$(echo "$memory_info" | tail -n1)
    
    total_memory=$((total_memory + memory_kb))
    if [ $memory_kb -gt $max_memory ]; then
        max_memory=$memory_kb
    fi

    if [ $exit_code -ne 0 ] && [ $exit_code -ne 1 ]; then
        fail_count=$((fail_count + 1))
        mv temp_test.op "$OUTPUT_DIR/error_$i.op"
        echo "$memory_info" > "$OUTPUT_DIR/error_$i.txt"
        log_message "Test $i failed (Memory: ${memory_kb}KB)"
    else
        success_count=$((success_count + 1))
        log_message "Test $i succeeded (Memory: ${memory_kb}KB)"
    fi
done

end_time=$(date +%s)
duration=$((end_time - start_time))
avg_memory=$((total_memory / NUM_TESTS))

cat > "$REPORT_FILE" << EOF
# Fuzzing Campaign Report

## Summary
- Total tests: $NUM_TESTS
- Successful tests: $success_count
- Failed tests: $fail_count
- Success rate: $(( (success_count * 100) / NUM_TESTS ))%

## Performance Metrics
- Duration: ${duration}s
- Average memory usage: ${avg_memory}KB
- Peak memory usage: ${max_memory}KB

## Details
Failed test files can be found in the \`$OUTPUT_DIR\` directory.
EOF

log_message "Fuzzing campaign completed! Report saved in: $REPORT_FILE"
rm -f temp_test.op