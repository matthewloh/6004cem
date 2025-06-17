#!/bin/bash

# ================================================================
# Static vs Dynamic Scheduling Analysis - Automation Script
# 6004CEM Parallel and Distributed Programming Coursework
# 
# Purpose: Dedicated script for running in-depth static vs dynamic
#          scheduling comparison with smaller vectors
# ================================================================

# Colors for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

# Script configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR" || exit 1

# Program configuration
PROGRAM_NAME="part_b_static_vs_dynamic"
SOURCE_FILE="part_b_vector_static_vs_dynamic.c"
OUTPUT_DIR="static_vs_dynamic_results"
REPORT_FILE="static_vs_dynamic_analysis_report.txt"

# Compiler settings for optimal performance
CC="gcc"
CFLAGS="-fopenmp -O3 -Wall -Wextra -march=native -mtune=native -ffast-math"
LIBS="-lm"

# System monitoring
LOAD_THRESHOLD=2.0  # Warn if system load is high

# Function to print colored output
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

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ $1${NC}"
}

print_emphasis() {
    echo -e "${PURPLE}${BOLD}$1${NC}"
}

# Function to check prerequisites
check_prerequisites() {
    print_header "CHECKING PREREQUISITES"
    
    local all_good=true
    
    # Check if source file exists
    if [ ! -f "$SOURCE_FILE" ]; then
        print_error "Source file $SOURCE_FILE not found!"
        echo "Please ensure the source file is in the current directory."
        all_good=false
    else
        print_success "Source file $SOURCE_FILE found"
    fi
    
    # Check compiler
    if ! command -v $CC &> /dev/null; then
        print_error "Compiler $CC not found!"
        all_good=false
    else
        print_success "Compiler $CC available: $(gcc --version | head -1)"
    fi
    
    # Check OpenMP support
    if $CC -fopenmp -v 2>&1 | grep -q "openmp"; then
        print_success "OpenMP support confirmed"
    else
        print_warning "OpenMP support may not be available"
    fi
    
    # Check system load
    local load_avg=$(uptime | awk -F'load average:' '{print $2}' | awk '{print $1}' | sed 's/,//')
    if (( $(echo "$load_avg > $LOAD_THRESHOLD" | bc -l) )); then
        print_warning "High system load detected ($load_avg). Results may be affected."
        print_info "Consider running this analysis when the system is less busy."
    else
        print_success "System load acceptable ($load_avg)"
    fi
    
    # Check available memory
    local mem_total=$(free -g | grep '^Mem:' | awk '{print $2}')
    local mem_available=$(free -g | grep '^Mem:' | awk '{print $7}')
    if [ "$mem_available" -lt 1 ]; then
        print_warning "Low memory available (${mem_available}GB of ${mem_total}GB)"
    else
        print_success "Memory available: ${mem_available}GB of ${mem_total}GB"
    fi
    
    echo
    
    if [ "$all_good" = false ]; then
        print_error "Prerequisites not met. Please fix the issues above."
        exit 1
    fi
    
    return 0
}

# Function to display comprehensive system information
display_system_info() {
    print_header "DETAILED SYSTEM INFORMATION"
    
    echo -e "${PURPLE}Machine Information:${NC}"
    echo "Hostname: $(hostname)"
    echo "User: $(whoami)"
    echo "Date: $(date)"
    echo "Working Directory: $(pwd)"
    echo "Kernel: $(uname -r)"
    echo "Distribution: $(cat /etc/os-release | grep PRETTY_NAME | cut -d'"' -f2 2>/dev/null || echo "Unknown")"
    echo
    
    echo -e "${PURPLE}Hardware Information:${NC}"
    echo "CPU Cores (logical): $(nproc)"
    echo "CPU Cores (physical): $(grep -c ^cpu\\scores /proc/cpuinfo 2>/dev/null || echo "unknown")"
    echo "CPU Model: $(grep 'model name' /proc/cpuinfo | head -1 | cut -d: -f2 | xargs)"
    echo "CPU Frequency: $(grep 'cpu MHz' /proc/cpuinfo | head -1 | cut -d: -f2 | xargs) MHz"
    echo "Cache Size: $(grep 'cache size' /proc/cpuinfo | head -1 | cut -d: -f2 | xargs)"
    echo "Memory Total: $(free -h | grep '^Mem:' | awk '{print $2}')"
    echo "Memory Available: $(free -h | grep '^Mem:' | awk '{print $7}')"
    echo
    
    echo -e "${PURPLE}Compiler and Environment:${NC}"
    echo "GCC Version: $(gcc --version | head -1)"
    echo "OpenMP Version: $(echo | gcc -fopenmp -dM -E - | grep -i openmp | head -1 || echo "Not detected")"
    echo "Compilation Flags: $CFLAGS"
    echo "Libraries: $LIBS"
    echo
    
    echo -e "${PURPLE}OpenMP Environment:${NC}"
    echo "OMP_NUM_THREADS: ${OMP_NUM_THREADS:-'not set (will use system default)'}"
    echo "OMP_SCHEDULE: ${OMP_SCHEDULE:-'not set (will use default)'}"
    echo "OMP_DYNAMIC: ${OMP_DYNAMIC:-'not set'}"
    echo "Max threads available: $(echo '#include <omp.h>
#include <stdio.h>
int main() { printf("%d\n", omp_get_max_threads()); return 0; }' | gcc -fopenmp -x c - -o /tmp/omp_test 2>/dev/null && /tmp/omp_test 2>/dev/null || echo "unknown")"
    rm -f /tmp/omp_test
    echo
    
    echo -e "${PURPLE}System Performance State:${NC}"
    echo "Load Average: $(uptime | awk -F'load average:' '{print $2}')"
    echo "CPU Governor: $(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor 2>/dev/null || echo "unknown")"
    echo "Swap Usage: $(free -h | grep '^Swap:' | awk '{print $3 "/" $2}')"
    echo
}

# Function to create output directory
setup_output_directory() {
    print_info "Setting up output directory..."
    
    if [ -d "$OUTPUT_DIR" ]; then
        print_warning "Output directory $OUTPUT_DIR already exists"
        read -p "Remove existing directory and start fresh? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            rm -rf "$OUTPUT_DIR"
            print_success "Removed existing output directory"
        fi
    fi
    
    mkdir -p "$OUTPUT_DIR"
    print_success "Output directory created: $OUTPUT_DIR"
}

# Function to compile the program with detailed feedback
compile_program() {
    print_header "COMPILING STATIC VS DYNAMIC ANALYSIS PROGRAM"
    
    print_info "Compiling $SOURCE_FILE with optimization flags..."
    print_info "Flags: $CFLAGS"
    
    # Clean any existing binary
    if [ -f "$PROGRAM_NAME" ]; then
        rm "$PROGRAM_NAME"
        print_info "Removed existing binary"
    fi
    
    # Compile with timing and detailed output
    local start_time=$(date +%s.%N)
    
    # Capture both stdout and stderr
    local compile_output=$(mktemp)
    if $CC $CFLAGS -o "$PROGRAM_NAME" "$SOURCE_FILE" $LIBS 2>&1 | tee "$compile_output"; then
        local end_time=$(date +%s.%N)
        local compile_time=$(echo "$end_time - $start_time" | bc 2>/dev/null || echo "unknown")
        
        print_success "Compilation successful (${compile_time}s)"
        
        # Check for warnings
        if grep -i "warning" "$compile_output" >/dev/null; then
            print_warning "Compilation warnings detected:"
            grep -i "warning" "$compile_output" | head -5
        fi
        
        # Display binary information
        local binary_size=$(stat -c%s "$PROGRAM_NAME" 2>/dev/null || echo "unknown")
        print_info "Binary size: $binary_size bytes"
        
        # Check if binary is executable
        if [ -x "$PROGRAM_NAME" ]; then
            print_success "Binary is executable"
        else
            print_error "Binary is not executable"
            rm -f "$compile_output"
            return 1
        fi
        
    else
        print_error "Compilation failed!"
        echo "Error details:"
        cat "$compile_output"
        rm -f "$compile_output"
        return 1
    fi
    
    rm -f "$compile_output"
    echo
    return 0
}

# Function to run performance analysis with monitoring
run_performance_analysis() {
    print_header "RUNNING STATIC VS DYNAMIC PERFORMANCE ANALYSIS"
    
    if [ ! -x "$PROGRAM_NAME" ]; then
        print_error "Executable $PROGRAM_NAME not found or not executable!"
        return 1
    fi
    
    print_info "Starting comprehensive performance analysis..."
    print_warning "This analysis includes:"
    echo "  • 4 vector sizes (1K, 10K, 50K, 100K elements)"
    echo "  • Multiple thread configurations (1, 2, 4, 8 threads)"
    echo "  • 10 runs per configuration for statistical accuracy"
    echo "  • Detailed chunk size optimization analysis"
    echo "  • Memory access pattern analysis"
    echo "  • Explanation of why dynamic might outperform static"
    echo
    
    print_warning "Expected runtime: 2-5 minutes depending on your system"
    echo
    
    read -p "Proceed with the analysis? (Y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Nn]$ ]]; then
        print_info "Analysis cancelled by user"
        return 0
    fi
    
    # Set optimal environment for testing
    export OMP_DYNAMIC=false  # Disable dynamic thread adjustment
    export OMP_NESTED=false   # Disable nested parallelism
    
    print_info "Environment configured for optimal testing"
    print_info "OMP_DYNAMIC=false, OMP_NESTED=false"
    echo
    
    # Create timestamped output file
    local timestamp=$(date +"%Y%m%d_%H%M%S")
    local output_file="$OUTPUT_DIR/analysis_output_$timestamp.txt"
    local timing_file="$OUTPUT_DIR/timing_log_$timestamp.txt"
    
    print_info "Output will be saved to: $output_file"
    print_info "Timing information: $timing_file"
    echo
    
    # Monitor system during execution
    {
        echo "Performance Analysis Execution Log"
        echo "================================="
        echo "Start time: $(date)"
        echo "Hostname: $(hostname)"
        echo "User: $(whoami)"
        echo "Load average at start: $(uptime | awk -F'load average:' '{print $2}')"
        echo
    } > "$timing_file"
    
    # Run the analysis with time monitoring
    local start_time=$(date +%s)
    print_emphasis "ANALYSIS STARTING - Monitor progress below:"
    echo
    
    # Run with both stdout and log capture
    if ./"$PROGRAM_NAME" 2>&1 | tee "$output_file"; then
        local end_time=$(date +%s)
        local total_time=$((end_time - start_time))
        
        {
            echo "Analysis completed successfully"
            echo "End time: $(date)"
            echo "Total execution time: ${total_time} seconds"
            echo "Load average at end: $(uptime | awk -F'load average:' '{print $2}')"
        } >> "$timing_file"
        
        print_success "Performance analysis completed in ${total_time} seconds"
        print_success "Results saved to: $output_file"
        
        # Basic result analysis
        analyze_results "$output_file"
        
    else
        print_error "Performance analysis failed!"
        {
            echo "Analysis failed"
            echo "End time: $(date)"
        } >> "$timing_file"
        return 1
    fi
    
    echo
    return 0
}

# Function to analyze and summarize results
analyze_results() {
    local output_file="$1"
    
    print_header "QUICK RESULTS ANALYSIS"
    
    if [ ! -f "$output_file" ]; then
        print_error "Output file not found: $output_file"
        return 1
    fi
    
    print_info "Analyzing results from: $output_file"
    echo
    
    # Count successful tests
    local static_tests=$(grep -c "Testing STATIC scheduling" "$output_file" 2>/dev/null || echo "0")
    local dynamic_tests=$(grep -c "Testing DYNAMIC scheduling" "$output_file" 2>/dev/null || echo "0")
    
    print_info "Test Summary:"
    echo "  Static scheduling tests: $static_tests"
    echo "  Dynamic scheduling tests: $dynamic_tests"
    
    # Check for verification success
    if grep -q "Static and Dynamic scheduling produce identical results" "$output_file"; then
        print_success "Results verification: PASSED (Static and Dynamic produce identical results)"
    else
        print_warning "Results verification: Could not confirm or may have failed"
    fi
    
    # Look for performance winners
    local static_wins=$(grep -c "Static (" "$output_file" 2>/dev/null || echo "0")
    local dynamic_wins=$(grep -c "Dynamic (" "$output_file" 2>/dev/null || echo "0")
    
    if [ "$static_wins" -gt 0 ] || [ "$dynamic_wins" -gt 0 ]; then
        echo
        print_info "Performance Summary:"
        echo "  Configurations where Static was faster: $static_wins"
        echo "  Configurations where Dynamic was faster: $dynamic_wins"
        
        if [ "$dynamic_wins" -gt "$static_wins" ]; then
            print_emphasis "→ Dynamic scheduling showed better performance in most cases"
        elif [ "$static_wins" -gt "$dynamic_wins" ]; then
            print_emphasis "→ Static scheduling showed better performance in most cases"
        else
            print_emphasis "→ Mixed results - performance varies by configuration"
        fi
    fi
    
    # Check if analysis sections completed
    if grep -q "ANALYSIS: WHY DYNAMIC SCHEDULING MIGHT BE FASTER" "$output_file"; then
        print_success "In-depth analysis sections completed"
    fi
    
    if grep -q "CONCLUSIONS AND RECOMMENDATIONS" "$output_file"; then
        print_success "Conclusions and recommendations generated"
    fi
    
    echo
}

# Function to generate comprehensive report
generate_report() {
    print_header "GENERATING COMPREHENSIVE REPORT"
    
    local report_path="$OUTPUT_DIR/$REPORT_FILE"
    
    {
        echo "=========================================="
        echo "Static vs Dynamic Scheduling Analysis Report"
        echo "=========================================="
        echo "Generated: $(date)"
        echo "Analysis Runtime: Approximately 2-5 minutes"
        echo
        echo "SYSTEM INFORMATION:"
        echo "Hostname: $(hostname)"
        echo "User: $(whoami)"
        echo "CPU Cores: $(nproc)"
        echo "CPU Model: $(grep 'model name' /proc/cpuinfo | head -1 | cut -d: -f2 | xargs)"
        echo "Memory: $(free -h | grep '^Mem:' | awk '{print $2}') total"
        echo "GCC Version: $(gcc --version | head -1)"
        echo
        echo "ANALYSIS CONFIGURATION:"
        echo "Source File: $SOURCE_FILE"
        echo "Executable: $PROGRAM_NAME"
        echo "Compilation Flags: $CFLAGS"
        echo "Vector Sizes Tested: 1K, 10K, 50K, 100K elements"
        echo "Thread Configurations: 1, 2, 4, 8 threads"
        echo "Runs per Configuration: 10 (for statistical accuracy)"
        echo
        echo "KEY FEATURES OF THIS ANALYSIS:"
        echo "✓ Focused comparison of static vs dynamic scheduling only"
        echo "✓ Smaller vector sizes for detailed analysis"
        echo "✓ Statistical analysis with standard deviation calculation"
        echo "✓ Chunk size optimization for dynamic scheduling"
        echo "✓ Memory access pattern analysis"
        echo "✓ Detailed explanation of performance differences"
        echo "✓ Work distribution visualization"
        echo "✓ System state monitoring during execution"
        echo
        echo "WHY THIS ANALYSIS IS VALUABLE:"
        echo "- Addresses the observation that dynamic scheduling often outperforms static"
        echo "- Uses smaller vectors where scheduling overhead is more significant"
        echo "- Provides statistical rigor with multiple runs"
        echo "- Explains the theoretical and practical reasons for performance differences"
        echo "- Optimizes chunk sizes for fair dynamic scheduling comparison"
        echo
        echo "COURSEWORK REQUIREMENTS ADDRESSED:"
        echo "✓ Vector addition with OpenMP work-sharing constructs"
        echo "✓ Static scheduling implementation and analysis"
        echo "✓ Dynamic scheduling implementation and analysis"
        echo "✓ Performance comparison and detailed analysis"
        echo "✓ Results verification and correctness checking"
        echo "✓ Comprehensive conclusions and recommendations"
        echo
        echo "SCREENSHOT REQUIREMENTS:"
        echo "📸 Take screenshots showing:"
        echo "   - Your hostname clearly visible in the terminal"
        echo "   - System verification information"
        echo "   - Performance comparison tables"
        echo "   - Results verification confirmations"
        echo "   - Analysis conclusions and recommendations"
        echo
        echo "FILES GENERATED:"
        echo "- Analysis output files in $OUTPUT_DIR/"
        echo "- Timing logs for performance monitoring"
        echo "- This comprehensive report"
        echo
        echo "NEXT STEPS:"
        echo "1. Review the detailed analysis output files"
        echo "2. Take required screenshots with hostname visible"
        echo "3. Analyze the performance differences and their explanations"
        echo "4. Use the findings to support your coursework conclusions"
        echo
    } > "$report_path"
    
    print_success "Comprehensive report generated: $report_path"
    print_info "Report includes system information, analysis configuration, and guidance"
}

# Function to show menu
show_menu() {
    echo
    print_header "STATIC VS DYNAMIC SCHEDULING ANALYSIS MENU"
    echo "1) Check Prerequisites and System Information"
    echo "2) Compile Static vs Dynamic Analysis Program"
    echo "3) Run Complete Performance Analysis"
    echo "4) Generate Comprehensive Report"
    echo "5) Run Full Analysis Pipeline (2 + 3 + 4)"
    echo "6) View Previous Results"
    echo "7) Clean Output Directory"
    echo "8) Exit"
    echo
}

# Function to view previous results
view_previous_results() {
    print_header "VIEWING PREVIOUS RESULTS"
    
    if [ ! -d "$OUTPUT_DIR" ]; then
        print_warning "No output directory found. Run analysis first."
        return
    fi
    
    local output_files=($(ls -t "$OUTPUT_DIR"/analysis_output_*.txt 2>/dev/null))
    
    if [ ${#output_files[@]} -eq 0 ]; then
        print_warning "No previous analysis results found."
        return
    fi
    
    print_info "Found ${#output_files[@]} previous analysis result(s):"
    echo
    
    for i in "${!output_files[@]}"; do
        local file="${output_files[$i]}"
        local file_date=$(stat -c %y "$file" | cut -d' ' -f1,2 | cut -d'.' -f1)
        local file_size=$(stat -c %s "$file")
        echo "$((i + 1)). $(basename "$file") - $file_date (${file_size} bytes)"
    done
    
    echo
    read -p "Select file to view (1-${#output_files[@]}) or Enter to return: " choice
    
    if [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge 1 ] && [ "$choice" -le ${#output_files[@]} ]; then
        local selected_file="${output_files[$((choice - 1))]}"
        print_info "Viewing: $(basename "$selected_file")"
        echo
        
        # Show last 50 lines with option to see more
        tail -50 "$selected_file"
        echo
        read -p "View full file? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            less "$selected_file"
        fi
    fi
}

# Function to clean output directory
clean_output() {
    print_header "CLEANING OUTPUT DIRECTORY"
    
    if [ ! -d "$OUTPUT_DIR" ]; then
        print_info "No output directory found. Nothing to clean."
        return
    fi
    
    local file_count=$(ls -1 "$OUTPUT_DIR" 2>/dev/null | wc -l)
    
    if [ "$file_count" -eq 0 ]; then
        print_info "Output directory is already empty."
        return
    fi
    
    print_warning "This will remove all $file_count file(s) from $OUTPUT_DIR/"
    read -p "Are you sure? (y/N): " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        rm -rf "$OUTPUT_DIR"/*
        print_success "Output directory cleaned"
    else
        print_info "Cleaning cancelled"
    fi
}

# Main function
main() {
    clear
    print_header "STATIC VS DYNAMIC SCHEDULING - AUTOMATED ANALYSIS"
    echo -e "${CYAN}This script provides comprehensive automation for analyzing the performance${NC}"
    echo -e "${CYAN}differences between static and dynamic scheduling in OpenMP vector addition.${NC}"
    echo
    echo -e "${PURPLE}Focus: Smaller vectors (1K-100K) with detailed statistical analysis${NC}"
    echo -e "${PURPLE}Goal: Understand why dynamic scheduling might outperform static${NC}"
    echo
    
    while true; do
        show_menu
        read -p "Select an option (1-8): " choice
        
        case $choice in
            1) 
                check_prerequisites
                display_system_info
                ;;
            2) 
                compile_program
                ;;
            3) 
                run_performance_analysis
                ;;
            4) 
                generate_report
                ;;
            5) 
                print_info "Running full analysis pipeline..."
                setup_output_directory && \
                compile_program && \
                run_performance_analysis && \
                generate_report && \
                print_success "Full analysis pipeline completed successfully!"
                ;;
            6) 
                view_previous_results
                ;;
            7) 
                clean_output
                ;;
            8) 
                print_success "Exiting Static vs Dynamic Analysis script"
                exit 0
                ;;
            *) 
                print_error "Invalid option. Please select 1-8."
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