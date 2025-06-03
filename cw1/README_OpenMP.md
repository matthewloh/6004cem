# OpenMP Coursework - Parallel Programming Portfolio

## Overview
This directory contains the three OpenMP programs required for the parallel programming portion of the coursework:

- **Part A**: Hello World with OpenMP (5% of portfolio grade)
- **Part B**: Vector Addition with Static/Dynamic Scheduling (20% of portfolio grade)  
- **Part C**: Matrix Calculation Performance Analysis (25% of portfolio grade)

## Files Included
- `part_a_hello_openmp.c` - Hello World program with thread control
- `part_b_vector_addition.c` - Vector addition with scheduling comparison
- `part_c_matrix_calculation.c` - Matrix multiplication performance analysis
- `Makefile` - Compilation instructions
- `README_OpenMP.md` - This documentation

## Compilation Instructions

### Prerequisites
- GCC compiler with OpenMP support
- Make utility (optional, but recommended)

### Using Make (Recommended)
```bash
# Compile all programs
make all

# Compile individual programs
make part_a_hello_openmp
make part_b_vector_addition
make part_c_matrix_calculation

# Clean compiled files
make clean
```

### Manual Compilation
```bash
# Part A
gcc -fopenmp -O2 -Wall -o part_a_hello_openmp part_a_hello_openmp.c -lm

# Part B
gcc -fopenmp -O2 -Wall -o part_b_vector_addition part_b_vector_addition.c -lm

# Part C
gcc -fopenmp -O2 -Wall -o part_c_matrix_calculation part_c_matrix_calculation.c -lm
```

## Running the Programs

### Part A: Hello World
```bash
./part_a_hello_openmp
# When prompted, enter desired number of threads (e.g., 8)
```

**Key Features:**
- Demonstrates 10 threads initially
- Shows environment variable control (simulated)
- Accepts user input for thread count
- Each thread prints its ID

### Part B: Vector Addition
```bash
./part_b_vector_addition
```

**Key Features:**
- Compares static vs dynamic scheduling
- Processes 1,000,000 element vectors
- Shows performance timing for both methods
- Provides detailed analysis of scheduling differences

### Part C: Matrix Calculation
```bash
./part_c_matrix_calculation
```

**Key Features:**
- Tests 50x50 and 500x500 matrices
- Uses 1, 4, 8, and 16 threads
- Compares outer vs inner loop parallelization
- Runs 10 iterations per test for accurate timing
- Calculates speedup and efficiency metrics

## Setting Environment Variables

### For Part A (Environment Variable Demo)
```bash
# On Linux/Mac
export OMP_NUM_THREADS=5
./part_a_hello_openmp

# On Windows PowerShell
$env:OMP_NUM_THREADS=5
./part_a_hello_openmp
```

## Key Points for Viva Discussion

### Part A - Thread Management
- **Thread Creation**: `#pragma omp parallel` creates parallel regions
- **Thread Control**: `omp_set_num_threads()` vs `OMP_NUM_THREADS`
- **Thread Identification**: `omp_get_thread_num()` and `omp_get_num_threads()`

### Part B - Work-Sharing Constructs
- **Static Scheduling**: 
  - Divides work evenly at compile time
  - Lower overhead, predictable assignment
  - Best for uniform workloads
- **Dynamic Scheduling**:
  - Assigns work chunks at runtime
  - Higher overhead, better load balancing
  - Best for irregular workloads
- **For this problem**: Vector addition is uniform, so static is more efficient

### Part C - Performance Analysis
- **Outer Loop Parallelization**: Better for matrix multiplication
  - Lower synchronization overhead
  - Better cache locality
  - More efficient thread utilization
- **Inner Loop Parallelization**: Less efficient
  - Higher overhead from frequent parallel regions
  - Poor performance due to overhead exceeding benefits
- **Scalability**: Performance gains more significant with larger matrices
- **Amdahl's Law**: Efficiency decreases as thread count increases

## Expected Results

### Performance Trends
1. **Speedup**: Should increase with more threads, but with diminishing returns
2. **Efficiency**: Typically decreases as thread count increases
3. **Matrix Size Impact**: Larger matrices show better parallelization benefits
4. **Outer vs Inner**: Outer loop parallelization should consistently outperform inner loop

### Common Observations
- Small matrices may show little to no speedup due to overhead
- Thread count beyond available cores may decrease performance
- Static scheduling generally faster for uniform workloads
- Cache effects become important with larger data sets

## Technical Concepts Demonstrated

### OpenMP Directives Used
- `#pragma omp parallel` - Create parallel regions
- `#pragma omp parallel for` - Parallel loop execution
- `#pragma omp critical` - Critical sections for thread-safe output
- `schedule(static)` and `schedule(dynamic)` - Loop scheduling policies

### Performance Metrics
- **Execution Time**: Using `omp_get_wtime()`
- **Speedup**: Serial time / Parallel time
- **Efficiency**: Speedup / Number of threads × 100%
- **Scalability**: Performance across different thread counts

### Memory Management
- Dynamic memory allocation for large data structures
- Proper memory cleanup to prevent leaks
- Consideration of memory access patterns for cache performance

## Troubleshooting

### Common Issues
1. **OpenMP not found**: Ensure GCC has OpenMP support (`gcc -fopenmp`)
2. **Poor performance**: Check if system has multiple cores available
3. **Memory issues**: Large matrices may require significant RAM
4. **Thread limits**: System may limit maximum thread count

### Performance Tips
- Run on a system with multiple cores for meaningful results
- Close other applications to get consistent timing results
- Run multiple times to account for system load variations
- Consider system architecture when interpreting results

## Analysis for Viva

Be prepared to discuss:
1. Why outer loop parallelization is more effective
2. The trade-offs between static and dynamic scheduling
3. How thread overhead affects small vs large problems
4. The relationship between problem size and parallelization benefits
5. Factors that limit scalability (Amdahl's Law)
6. Cache locality and memory access patterns
7. Load balancing considerations

## Screenshots Required
Don't forget to take screenshots of all programs running with your machine name visible for submission! 