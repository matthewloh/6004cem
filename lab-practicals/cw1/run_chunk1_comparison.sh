#!/bin/bash

# ================================================================
# Static vs Dynamic(chunk=1) Comparison - Simple Runner
# 6004CEM Parallel and Distributed Programming Coursework
# ================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

# Configuration
PROGRAM_NAME="static_vs_dynamic_chunk1"
SOURCE_FILE="part_b_static_vs_dynamic_chunk1.c"
OUTPUT_FILE="chunk1_comparison_results.txt"

# Print functions
print_header() {
    echo -e "${CYAN}${BOLD}================================================================${NC}"
    echo -e "${CYAN}${BOLD}$1${NC}"
    echo -e "${CYAN}${BOLD}================================================================${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ $1${NC}"
}

# Main execution
clear
print_header "STATIC vs DYNAMIC(chunk=1) SCHEDULING COMPARISON"

echo -e "${PURPLE}This program compares static scheduling against dynamic scheduling${NC}"
echo -e "${PURPLE}with chunk size 1, showing the extreme case of fine-grained work distribution.${NC}"
echo

# Check if source file exists
if [ ! -f "$SOURCE_FILE" ]; then
    print_error "Source file $SOURCE_FILE not found!"
    exit 1
fi

print_success "Source file found: $SOURCE_FILE"

# Compile the program
print_info "Compiling with optimization flags..."
if gcc -fopenmp -O3 -Wall -Wextra -march=native -o "$PROGRAM_NAME" "$SOURCE_FILE" -lm; then
    print_success "Compilation successful"
else
    print_error "Compilation failed!"
    exit 1
fi

# Check if user wants to save output
read -p "Save output to file? (Y/n): " -n 1 -r
echo
if [[ $REPLY =~ ^[Nn]$ ]]; then
    SAVE_OUTPUT=false
else
    SAVE_OUTPUT=true
fi

# Set up environment for optimal testing
export OMP_DYNAMIC=false
export OMP_NESTED=false

print_info "Environment configured: OMP_DYNAMIC=false, OMP_NESTED=false"
print_info "This analysis will take approximately 3-5 minutes..."
echo

# Run the program
print_header "STARTING ANALYSIS"

if [ "$SAVE_OUTPUT" = true ]; then
    print_info "Results will be saved to: $OUTPUT_FILE"
    echo
    
    # Add header to output file
    {
        echo "Static vs Dynamic(chunk=1) Scheduling Comparison Results"
        echo "======================================================="
        echo "Generated: $(date)"
        echo "Hostname: $(hostname)"
        echo "User: $(whoami)"
        echo "System: $(uname -a)"
        echo
    } > "$OUTPUT_FILE"
    
    # Run program and save output
    if ./"$PROGRAM_NAME" 2>&1 | tee -a "$OUTPUT_FILE"; then
        print_success "Analysis completed successfully!"
        print_success "Results saved to: $OUTPUT_FILE"
    else
        print_error "Analysis failed!"
        exit 1
    fi
else
    # Run program without saving
    if ./"$PROGRAM_NAME"; then
        print_success "Analysis completed successfully!"
    else
        print_error "Analysis failed!"
        exit 1
    fi
fi

echo
print_header "ANALYSIS COMPLETE"
echo -e "${YELLOW}Key points to note in your coursework:${NC}"
echo "• Static scheduling typically outperforms Dynamic(chunk=1)"
echo "• Dynamic(chunk=1) has very high synchronization overhead"
echo "• Cache performance strongly favors static scheduling"
echo "• This demonstrates why chunk size selection is critical"
echo "• Perfect example of scheduling trade-offs in practice"

if [ "$SAVE_OUTPUT" = true ]; then
    echo
    echo -e "${PURPLE}📸 Screenshot Requirements:${NC}"
    echo "• Take screenshots showing your hostname in the terminal"
    echo "• Capture the performance comparison tables"
    echo "• Show the theoretical analysis sections"
    echo "• Include the final conclusions and recommendations"
fi

echo
print_success "Static vs Dynamic(chunk=1) comparison completed!" 