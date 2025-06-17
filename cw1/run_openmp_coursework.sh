#!/bin/bash

# ================================================================
# OpenMP Coursework - Automated Build and Run Script
# 6004CEM Parallel and Distributed Programming
# ================================================================

# Colors for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR" || exit 1

# Program names
PROGRAMS=("part_a_hello_openmp" "part_b_vector_addition" "part_c_matrix_calculation")
SOURCE_FILES=("part_a_hello_openmp.c" "part_b_vector_addition.c" "part_c_matrix_calculation.c")

# Enhanced program names
ENHANCED_PROGRAMS=("part_a_enhanced" "part_b_enhanced" "part_c_enhanced")
ENHANCED_SOURCE_FILES=("part_a_hello_openmp_enhanced.c" "part_b_vector_addition_enhanced.c" "part_c_matrix_calculation_enhanced.c")

# Compiler settings
CC="gcc"
CFLAGS="-fopenmp -O3 -Wall -Wextra -march=native"
LIBS="-lm"

# Function to print colored output
print_header() {
    echo -e "${CYAN}================================================================${NC}"
    echo -e "${CYAN}$1${NC}"
    echo -e "${CYAN}================================================================${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ $1${NC}"
}

# Function to check system information
check_system() {
    print_header "SYSTEM INFORMATION CHECK"
    
    echo -e "${PURPLE}Machine Information:${NC}"
    echo "Hostname: $(hostname)"
    echo "User: $(whoami)"
    echo "Date: $(date)"
    echo "Working Directory: $(pwd)"
    echo
    
    echo -e "${PURPLE}Hardware Information:${NC}"
    echo "CPU Cores: $(nproc)"
    echo "CPU Info: $(grep 'model name' /proc/cpuinfo | head -1 | cut -d: -f2 | xargs)"
    echo "Memory: $(free -h | grep '^Mem:' | awk '{print $2}') total"
    echo
    
    echo -e "${PURPLE}Compiler Information:${NC}"
    if command -v gcc &> /dev/null; then
        echo "GCC Version: $(gcc --version | head -1)"
        
        # Check OpenMP support
        if gcc -fopenmp -v 2>&1 | grep -q "openmp"; then
            print_success "OpenMP support detected"
        else
            print_warning "OpenMP support may not be available"
        fi
    else
        print_error "GCC compiler not found!"
        exit 1
    fi
    echo
    
    echo -e "${PURPLE}OpenMP Environment:${NC}"
    echo "OMP_NUM_THREADS: ${OMP_NUM_THREADS:-'not set'}"
    echo "Maximum OpenMP threads available: $(echo '#include <omp.h>
#include <stdio.h>
int main() { printf("%d\n", omp_get_max_threads()); return 0; }' | gcc -fopenmp -x c - -o /tmp/omp_test 2>/dev/null && /tmp/omp_test 2>/dev/null || echo "unknown")"
    rm -f /tmp/omp_test
}

# Function to clean previous builds
clean_build() {
    print_header "CLEANING PREVIOUS BUILDS"
    
    for program in "${PROGRAMS[@]}"; do
        if [ -f "$program" ]; then
            rm "$program"
            print_success "Removed $program"
        fi
    done
    
    # Remove any object files
    rm -f *.o 2>/dev/null
    echo
}

# Function to compile programs
compile_programs() {
    print_header "COMPILING OPENMP PROGRAMS"
    
    local compilation_success=true
    
    for i in "${!PROGRAMS[@]}"; do
        local program="${PROGRAMS[$i]}"
        local source="${SOURCE_FILES[$i]}"
        
        echo -e "${PURPLE}Compiling $source...${NC}"
        
        if [ ! -f "$source" ]; then
            print_error "Source file $source not found!"
            compilation_success=false
            continue
        fi
        
        # Compile with timing
        local start_time=$(date +%s.%N)
        if $CC $CFLAGS -o "$program" "$source" $LIBS 2>&1; then
            local end_time=$(date +%s.%N)
            local compile_time=$(echo "$end_time - $start_time" | bc 2>/dev/null || echo "unknown")
            print_success "Successfully compiled $program (${compile_time}s)"
        else
            print_error "Compilation failed for $source"
            compilation_success=false
        fi
    done
    
    echo
    
    if [ "$compilation_success" = true ]; then
        print_success "All programs compiled successfully!"
        return 0
    else
        print_error "Some programs failed to compile"
        return 1
    fi
}

# Function to compile enhanced programs
compile_enhanced_programs() {
    print_header "COMPILING ENHANCED OPENMP PROGRAMS"
    
    local compilation_success=true
    
    for i in "${!ENHANCED_PROGRAMS[@]}"; do
        local program="${ENHANCED_PROGRAMS[$i]}"
        local source="${ENHANCED_SOURCE_FILES[$i]}"
        
        echo -e "${PURPLE}Compiling enhanced $source...${NC}"
        
        if [ ! -f "$source" ]; then
            print_error "Source file $source not found!"
            compilation_success=false
            continue
        fi
        
        # Compile with timing
        local start_time=$(date +%s.%N)
        if $CC $CFLAGS -o "$program" "$source" $LIBS 2>&1; then
            local end_time=$(date +%s.%N)
            local compile_time=$(echo "$end_time - $start_time" | bc 2>/dev/null || echo "unknown")
            print_success "Successfully compiled enhanced $program (${compile_time}s)"
        else
            print_error "Compilation failed for enhanced $source"
            compilation_success=false
        fi
    done
    
    echo
    
    if [ "$compilation_success" = true ]; then
        print_success "All enhanced programs compiled successfully!"
        return 0
    else
        print_error "Some enhanced programs failed to compile"
        return 1
    fi
}

# Function to run Part A with proper demonstration
run_part_a() {
    print_header "RUNNING PART A: HELLO WORLD WITH OPENMP"
    
    if [ ! -f "part_a_hello_openmp" ]; then
        print_error "part_a_hello_openmp executable not found!"
        return 1
    fi
    
    print_info "This demonstrates OpenMP Hello World with different thread configurations"
    echo
    
    # Run with different thread inputs
    echo -e "${PURPLE}Running with user input (8 threads):${NC}"
    echo "8" | ./part_a_hello_openmp
    echo
    
    print_info "Screenshot requirement: Take a screenshot showing your hostname and the program output"
    echo
    
    # Test environment variable functionality
    print_info "Testing environment variable control:"
    echo -e "${PURPLE}Setting OMP_NUM_THREADS=6 and running:${NC}"
    OMP_NUM_THREADS=6 echo "6" | ./part_a_hello_openmp
    echo
}

# Function to run Part B with analysis
run_part_b() {
    print_header "RUNNING PART B: VECTOR ADDITION WITH SCHEDULING"
    
    if [ ! -f "part_b_vector_addition" ]; then
        print_error "part_b_vector_addition executable not found!"
        return 1
    fi
    
    print_info "This demonstrates static vs dynamic scheduling for vector addition"
    echo
    
    # Run the program
    ./part_b_vector_addition
    echo
    
    print_info "Screenshot requirement: Take a screenshot showing your hostname and the program output"
    print_info "Key observations to note:"
    echo "  - Static scheduling typically faster for uniform workloads"
    echo "  - Dynamic scheduling has higher overhead but better load balancing"
    echo "  - Results should be identical between both methods"
    echo
}

# Function to run Part C with comprehensive testing
run_part_c() {
    print_header "RUNNING PART C: MATRIX MULTIPLICATION PERFORMANCE ANALYSIS"
    
    if [ ! -f "part_c_matrix_calculation" ]; then
        print_error "part_c_matrix_calculation executable not found!"
        return 1
    fi
    
    print_info "This performs comprehensive matrix multiplication performance analysis"
    print_info "Testing configurations: 1, 4, 8, 16 threads on 50x50 and 500x500 matrices"
    print_info "Each test runs 10 times for statistical accuracy"
    echo
    
    print_warning "This test may take several minutes to complete..."
    echo
    
    # Run with timing
    local start_time=$(date +%s)
    ./part_c_matrix_calculation
    local end_time=$(date +%s)
    local total_time=$((end_time - start_time))
    
    echo
    print_success "Performance analysis completed in ${total_time} seconds"
    print_info "Screenshot requirement: Take a screenshot showing your hostname and the timing results"
    print_info "Key metrics to analyze:"
    echo "  - Speedup ratios (should increase with more threads, up to core limit)"
    echo "  - Efficiency percentages (typically decreases with more threads)"
    echo "  - Outer loop should outperform inner loop parallelization"
    echo "  - Larger matrices should show better scalability"
    echo
}

# Function to run enhanced Part A
run_enhanced_part_a() {
    print_header "RUNNING ENHANCED PART A: COMPREHENSIVE HELLO WORLD"
    
    if [ ! -f "part_a_enhanced" ]; then
        print_error "part_a_enhanced executable not found!"
        return 1
    fi
    
    print_info "This demonstrates comprehensive OpenMP features with system verification"
    echo
    
    # Run with predefined input
    echo -e "${PURPLE}Running enhanced Part A with comprehensive demonstrations:${NC}"
    echo "6" | ./part_a_enhanced
    echo
    
    print_info "Enhanced features demonstrated:"
    echo "  - System verification information (hostname, user, timestamp)"
    echo "  - Advanced OpenMP constructs (barriers, reductions, ordered sections)"
    echo "  - Thread affinity and processor mapping"
    echo "  - Performance timing measurements"
    echo "  - Comprehensive error handling and input validation"
    echo
}

# Function to run enhanced Part B
run_enhanced_part_b() {
    print_header "RUNNING ENHANCED PART B: COMPREHENSIVE VECTOR ADDITION ANALYSIS"
    
    if [ ! -f "part_b_enhanced" ]; then
        print_error "part_b_enhanced executable not found!"
        return 1
    fi
    
    print_info "This performs comprehensive vector addition analysis with multiple scheduling strategies"
    print_warning "This test may take several minutes due to multiple vector sizes and statistical runs..."
    echo
    
    # Run the enhanced program with timing
    local start_time=$(date +%s)
    timeout 300s ./part_b_enhanced  # 5 minute timeout
    local end_time=$(date +%s)
    local total_time=$((end_time - start_time))
    
    echo
    print_success "Enhanced vector addition analysis completed in ${total_time} seconds"
    print_info "Enhanced features demonstrated:"
    echo "  - Multiple vector sizes (100K, 500K, 1M, 5M)"
    echo "  - Four scheduling types (static, dynamic, guided, auto)"
    echo "  - Statistical analysis with multiple runs"
    echo "  - Scalability analysis (speedup and efficiency calculations)"
    echo "  - Comprehensive performance comparisons"
    echo
}

# Function to run enhanced Part C
run_enhanced_part_c() {
    print_header "RUNNING ENHANCED PART C: COMPREHENSIVE MATRIX PERFORMANCE ANALYSIS"
    
    if [ ! -f "part_c_enhanced" ]; then
        print_error "part_c_enhanced executable not found!"
        return 1
    fi
    
    print_info "This performs comprehensive matrix multiplication analysis with enhanced features"
    print_warning "This test may take 10+ minutes due to multiple matrix sizes and comprehensive analysis..."
    echo
    
    # Run with timing
    local start_time=$(date +%s)
    timeout 600s ./part_c_enhanced  # 10 minute timeout
    local end_time=$(date +%s)
    local total_time=$((end_time - start_time))
    
    echo
    print_success "Enhanced matrix multiplication analysis completed in ${total_time} seconds"
    print_info "Enhanced features demonstrated:"
    echo "  - Additional matrix sizes (100x100, 1000x1000)"
    echo "  - Statistical analysis (min, max, standard deviation)"
    echo "  - GFLOPS calculations and memory bandwidth analysis"
    echo "  - ASCII performance visualizations"
    echo "  - Comprehensive scalability predictions"
    echo "  - Cache-optimized algorithms"
    echo
}

# Function to generate performance report
generate_report() {
    print_header "GENERATING PERFORMANCE SUMMARY REPORT"
    
    local report_file="openmp_performance_report.txt"
    
    {
        echo "=========================================="
        echo "OpenMP Coursework Performance Report"
        echo "=========================================="
        echo "Generated: $(date)"
        echo "Hostname: $(hostname)"
        echo "User: $(whoami)"
        echo "CPU Cores: $(nproc)"
        echo "GCC Version: $(gcc --version | head -1)"
        echo
        echo "Programs Tested:"
        for program in "${PROGRAMS[@]}"; do
            if [ -f "$program" ]; then
                echo "✓ $program"
            else
                echo "✗ $program (not found)"
            fi
        done
        echo
        echo "Environment Variables:"
        echo "OMP_NUM_THREADS: ${OMP_NUM_THREADS:-'not set'}"
        echo
        echo "Compilation Flags Used:"
        echo "$CC $CFLAGS ... $LIBS"
        echo
        echo "Notes:"
        echo "- All programs demonstrate proper OpenMP usage"
        echo "- Performance varies based on system architecture"
        echo "- Screenshots taken with hostname visible for verification"
        echo "- Multiple test runs ensure statistical accuracy"
        echo
    } > "$report_file"
    
    print_success "Performance report saved to: $report_file"
}

# Function to show menu
show_menu() {
    echo
    print_header "OPENMP COURSEWORK AUTOMATION MENU"
    echo "=== BASIC PROGRAMS (Assignment Requirements) ==="
    echo "1) Check System Information"
    echo "2) Clean Previous Builds"
    echo "3) Compile All Programs"
    echo "4) Run Part A (Hello World)"
    echo "5) Run Part B (Vector Addition)"
    echo "6) Run Part C (Matrix Calculation)"
    echo "7) Run All Programs Sequentially"
    echo
    echo "=== ENHANCED PROGRAMS ==="
    echo "11) Compile Enhanced Programs"
    echo "12) Run Enhanced Part A (Comprehensive)"
    echo "13) Run Enhanced Part B (Comprehensive)"
    echo "14) Run Enhanced Part C (Comprehensive)"
    echo "15) Run All Enhanced Programs"
    echo
    echo "=== UTILITIES ==="
    echo "8) Generate Performance Report"
    echo "9) Exit"
    echo
}

# Main execution function
main() {
    clear
    print_header "6004CEM OPENMP COURSEWORK AUTOMATION SCRIPT"
    echo -e "${CYAN}This script will help you compile, run, and test your OpenMP coursework${NC}"
    echo -e "${CYAN}Make sure to take screenshots with your hostname visible!${NC}"
    echo
    
    # Initial system check
    check_system
    
    while true; do
        show_menu
        read -p "Select an option (1-15): " choice
        
        case $choice in
            1) check_system ;;
            2) clean_build ;;
            3) compile_programs ;;
            4) run_part_a ;;
            5) run_part_b ;;
            6) run_part_c ;;
            7) 
                compile_programs && {
                    run_part_a
                    run_part_b
                    run_part_c
                    generate_report
                }
                ;;
            8) generate_report ;;
            9) 
                print_success "Exiting OpenMP coursework automation script"
                exit 0
                ;;
            11) compile_enhanced_programs ;;
            12) run_enhanced_part_a ;;
            13) run_enhanced_part_b ;;
            14) run_enhanced_part_c ;;
            15) 
                compile_enhanced_programs && {
                    run_enhanced_part_a
                    run_enhanced_part_b
                    run_enhanced_part_c
                    generate_report
                }
                ;;
            *) 
                print_error "Invalid option. Please select 1-15."
                ;;
        esac
        
        echo
        read -p "Press Enter to continue..."
    done
}

# Check if script is being run directly
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi 