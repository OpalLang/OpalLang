#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

BENCH_DIR="benchmark_tests"
mkdir -p $BENCH_DIR

echo -e "${YELLOW}🚀 Starting benchmark suite...${NC}"

generate_test_file() {
    local size=$1
    local filename="$BENCH_DIR/test_${size}.op"
    
    echo -e "${YELLOW}📝 Generating ${size} test file...${NC}"
    
    {
        echo "// Complex program to test lexer performance"
        echo "class BenchmarkTest {"
        echo "    fn fibonacci(n) {"
        echo "        if n <= 1 {"
        echo "            ret n"
        echo "        }"
        echo "        ret fibonacci(n - 1) + fibonacci(n - 2)"
        echo "    }"
        echo "}"

        echo "fn process_range(start, end) {"
        echo "    foreach i in start..end step 2 {"
        echo "        /* Nested comment level 1"
        echo "           /* Nested comment level 2 */"
        echo "           Still in level 1"
        echo "        */"
        echo "    }"
        echo "}"

        for ((i=0; i<size; i++)); do
            # Generate variable declarations with string interpolation
            echo "    let str_${i} = \"Value at ${i}: \${fibonacci(${i} % 10)}\""
            
            # Generate complex expressions
            echo "    let complex_${i} = (${i} * 3.14159) ^ 2 + fibonacci(${i} % 5)"
            
            # Add different types of control structures
            if ((i % 7 == 0)); then
                echo "    if complex_${i} > 100 {"
                echo "        // Optimization point ${i}"
                echo "        try {"
                echo "            process_range(0, complex_${i})"
                echo "        } catch (error) {"
                echo "            // Error handling for test ${i}"
                echo "        } finally {"
                echo "            complex_${i} = complex_${i} - 1"
                echo "        }"
                echo "    } elif complex_${i} > 50 {"
                echo "        while complex_${i} > 0 {"
                echo "            complex_${i} = complex_${i} - 1"
                echo "        }"
                echo "    } else {"
                echo "        foreach x in 0..10 {"
                echo "            complex_${i} = complex_${i} + x"
                echo "        }"
                echo "    }"
            fi
            
            # Add array operations
            if ((i % 5 == 0)); then
                echo "    let array_${i} = [1, 2, 3, \"test\", fibonacci(${i} % 3)]"
                echo "    foreach item in array_${i} {"
                echo "        // Processing array item"
                echo "        complex_${i} = complex_${i} + 1"
                echo "    }"
            fi
        done
        
        echo "fn main() {"
        echo "    let test = BenchmarkTest()"
        echo "    let result = 0"
        echo "    foreach i in 0..${size} {"
        echo "        result = result + test.fibonacci(i % 10)"
        echo "    }"
        echo "    ret result"
        echo "}"
    } > "$filename"
}

run_benchmark() {
    local size=$1
    local file="$BENCH_DIR/test_${size}.op"
    
    echo -e "${YELLOW}🔍 Running benchmark for size ${size}...${NC}"
    
    start_time=$(date +%s.%N)
    ./bin/opal "$file" > /dev/null 2>&1
    end_time=$(date +%s.%N)
    
    execution_time=$(echo "$end_time - $start_time" | bc)
    echo -e "${GREEN}✓ Size ${size}: ${execution_time} seconds${NC}"
    
    echo "$size,$execution_time" >> "$BENCH_DIR/results.csv"
}

echo "size,time" > "$BENCH_DIR/results.csv"

if [ ! -f "./bin/opal" ]; then
    echo -e "${YELLOW}🔨 Building project...${NC}"
    ./scripts/build.sh
fi

test_sizes=(100 1000 5000 10000 50000)

for size in "${test_sizes[@]}"; do
    generate_test_file $size
    run_benchmark $size
done

echo -e "${GREEN}✅ Benchmark complete! Results saved in ${BENCH_DIR}/results.csv${NC}"
echo -e "${YELLOW}📊 Summary:${NC}"
column -t -s, "$BENCH_DIR/results.csv"