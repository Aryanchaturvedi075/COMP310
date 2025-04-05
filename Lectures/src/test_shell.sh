#!/bin/bash

# Colors for better readability
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Variables to track test results
total_tests=0
passed_tests=0
failed_tests=0

# Create temporary directory for test outputs
mkdir -p test_output

# Record initial directory state for cleanup later
find . -not -path "./test_output*" | sort > test_output/initial_state.txt

echo -e "${BLUE}Starting test sequence for mysh${NC}"
echo "====================================="

# Change into the test-cases directory for execution of program
echo "Changing into test-cases directory"
cd test-cases

# Find all test files (excluding _result.txt files)
test_files=$(find . -name "*.txt" | grep -v "_result.txt" | sort)

# Run each test
for test_file in $test_files; do
    # Get base filename and corresponding result file
    base_name=$(basename "$test_file" .txt)
    # dir_name=$(dirname "$test_file")
    # result_file="${dir_name}/${base_name}_result.txt"
    result_file="${base_name}_result.txt"
    output_file="../test_output/${base_name}_output.txt"
    
    echo -e "${BLUE}Running test:${NC} $base_name"
    
    # Execute test through mysh and capture output
    "../../project/src/mysh" < "$test_file" > "$output_file" 2>&1
    
    # Compare output with expected result, ignoring whitespace
    diff -w -B "$output_file" "$result_file" > "../test_output/${base_name}_diff.txt"
    
    # Check result
    if [ $? -eq 0 ]; then
        echo -e "  ${GREEN}PASSED${NC}"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "  ${RED}FAILED${NC}"
        echo "  Differences saved to: test_output/${base_name}_diff.txt"
        failed_tests=$((failed_tests + 1))
    fi
    
    total_tests=$((total_tests + 1))
done

# Change back to directory where this test scripts exists
cd ..

# Record final directory state
find . -not -path "./test_output*" | sort > test_output/final_state.txt

# Clean up any created files/directories
echo -e "\n${BLUE}Cleaning up test artifacts...${NC}"
comm -13 test_output/initial_state.txt test_output/final_state.txt > test_output/to_clean.txt

if [ -s test_output/to_clean.txt ]; then
    while read -r item; do
        if [ -e "$item" ]; then
            echo "  Removing: $item"
            rm -rf "$item"
        fi
    done < test_output/to_clean.txt
else
    echo "  No artifacts to clean."
fi

# Display test summary
echo -e "\n${BLUE}Test Summary${NC}"
echo "====================================="
echo "Total tests: $total_tests"
echo -e "${GREEN}Passed:${NC} $passed_tests"
echo -e "${RED}Failed:${NC} $failed_tests"
echo "====================================="

# Keep the output directory for review
echo "Test outputs saved in 'test_output' directory"

# Exit with success status if all tests passed
if [ $failed_tests -eq 0 ]; then
    exit 0
else
    exit 1
fi
