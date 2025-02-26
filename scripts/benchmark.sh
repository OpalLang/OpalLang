#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

TIMEFORMAT='%3R'
TEST_SIZES=(1000 5000 10000 50000)
OUTPUT_DIR="benchmark_results"
MEMORY_LOG="$OUTPUT_DIR/memory_usage.log"


rm -f "$OUTPUT_DIR/results.txt" "$MEMORY_LOG"
mkdir -p "$OUTPUT_DIR"

measure_memory() {
    local pid=$1
    local output_file=$2
    > "$output_file"
    
    ps -o rss= -p $pid | tr -d ' ' >> "$output_file"
    
    while kill -0 $pid 2>/dev/null; do
        ps -o rss= -p $pid | tr -d ' ' >> "$output_file"
        sleep 0.01
    done
    
    if kill -0 $pid 2>/dev/null; then
        ps -o rss= -p $pid | tr -d ' ' >> $output_file
    fi
}

generate_test_file() {
    local size=$1
    local output_file="benchmark_tests/test_${size}.op"
    
    mkdir -p benchmark_tests
    
    cat > "$output_file" << EOL
/* Benchmark test for size $size */
fn main() {
    test_size = $size
    start_time = time()
EOL

    echo "Generating $size test cases..."

    for ((i=0; i<size; i++)); do
        echo "    str_${i} = \"Value at ${i}: \${fibonacci(${i} % 10)}\"" >> "$output_file"
        echo "    complex_${i} = (${i} * 3.14159) ^ 2 + fibonacci(${i} % 5)" >> "$output_file"
        
        if ((i % 5 == 0)); then
            echo "    array_${i} = [1, 2, 3, \"test\", fibonacci(${i} % 3)]" >> "$output_file"
            echo "    foreach item in array_${i} {" >> "$output_file"
            echo "        complex_${i} = complex_${i} + 1" >> "$output_file"
            echo "    }" >> "$output_file"
        fi
        
        if ((i % 7 == 0)); then
            echo "    if complex_${i} > 100 {" >> "$output_file"
            echo "        try {" >> "$output_file"
            echo "            process_range(0, complex_${i})" >> "$output_file"
            echo "        } catch (error) {" >> "$output_file"
            echo "            complex_${i} = complex_${i} - 1" >> "$output_file"
            echo "        } finally {" >> "$output_file"
            echo "            complex_${i} = complex_${i} - 1" >> "$output_file"
            echo "        }" >> "$output_file"
            echo "    } elif complex_${i} > 50 {" >> "$output_file"
            echo "        while complex_${i} > 0 {" >> "$output_file"
            echo "            complex_${i} = complex_${i} - 1" >> "$output_file"
            echo "        }" >> "$output_file"
            echo "    } else {" >> "$output_file"
            echo "        for x in 0..10 {" >> "$output_file"
            echo "            complex_${i} = complex_${i} + x" >> "$output_file"
            echo "        }" >> "$output_file"
            echo "    }" >> "$output_file"
        fi
    done

    cat >> "$output_file" << EOL

    end_time = time()
    execution_time = end_time - start_time
    output_file = open("\${OUTPUT_DIR}/benchmark_\${test_size}.txt", "w")
    output_file.write("Benchmark completed:\\n")
    output_file.write("- Size: \${test_size}\\n")
    output_file.write("- Execution time: \${execution_time}s\\n")
    output_file.write("- Memory usage: \${get_memory_usage()}MB\\n")
    output_file.close()
}
EOL
}

echo -e "${BLUE}🚀 Starting benchmarks...${NC}"
for size in "${TEST_SIZES[@]}"; do
    echo -e "${YELLOW}⚙️  Running benchmark for size $size...${NC}"
    
    generate_test_file $size
    
    START_TIME=$(date +%s.%N)
    
    TEMP_MEM_LOG="${MEMORY_LOG}.${size}"
    > "$TEMP_MEM_LOG"

    ./bin/opal benchmark_tests/test_${size}.op >/dev/null 2>&1 &
    PROC_PID=$!
    measure_memory $PROC_PID "$TEMP_MEM_LOG" &
    MEMORY_PID=$!
    
    wait $PROC_PID
    wait $MEMORY_PID 2>/dev/null
    
    END_TIME=$(date +%s.%N)
    DURATION=$(echo "$END_TIME - $START_TIME" | bc)
    

    MAX_MEMORY=$(sort -n "$TEMP_MEM_LOG" | tail -n1)
    cat "$TEMP_MEM_LOG" >> "$MEMORY_LOG"
    rm -f "$TEMP_MEM_LOG"
    
    if [ -z "$MAX_MEMORY" ]; then
        MAX_MEMORY=0
    fi
    
    echo "Size: $size, Time: ${DURATION}s, Memory: ${MAX_MEMORY}KB" >> "$OUTPUT_DIR/results.txt"
    echo -e "${GREEN}✅ Completed size $size${NC}"
done

echo -e "${BLUE}📊 Generating final report...${NC}"
echo "# Benchmark Results 📈" > "$OUTPUT_DIR/report.md"
echo "" >> "$OUTPUT_DIR/report.md"
echo "| Test Size | Time (s) | Memory (KB) |" >> "$OUTPUT_DIR/report.md"
echo "|-----------|----------|-------------|" >> "$OUTPUT_DIR/report.md"
while read -r line; do
    size=$(echo $line | cut -d',' -f1 | cut -d':' -f2 | tr -d ' ')
    time=$(echo $line | cut -d',' -f2 | cut -d':' -f2 | tr -d ' ')
    memory=$(echo $line | cut -d',' -f3 | cut -d':' -f2 | tr -d ' KB')
    echo "| $size | $time | $memory |" >> "$OUTPUT_DIR/report.md"
done < "$OUTPUT_DIR/results.txt"

echo -e "${GREEN}✨ Benchmarks completed. Results available in $OUTPUT_DIR/${NC}"