# OpenMP Viva Preparation Summary

## Quick Reference for Your Viva Tomorrow

### What I've Created for You

✅ **Part A** - `part_a_hello_openmp.c` (5% grade)
- Hello World with 10 threads
- Environment variable control (OMP_NUM_THREADS)
- User input for thread count
- Thread ID identification

✅ **Part B** - `part_b_vector_addition.c` (20% grade)  
- Vector addition with 1M elements
- Static vs Dynamic scheduling comparison
- Performance timing and analysis
- Work-sharing construct demonstration

✅ **Part C** - `part_c_matrix_calculation.c` (25% grade)
- Matrix multiplication (50x50 and 500x500)
- Tests 1, 4, 8, 16 threads
- Outer vs Inner loop parallelization
- 10 runs each with timing tables
- Speedup and efficiency calculations

✅ **Support Files**
- `Makefile` for Linux/Mac compilation
- `compile_windows.bat` for Windows compilation
- `README_OpenMP.md` with detailed instructions
- This viva summary

## Key Concepts to Know Cold

### OpenMP Basics
```c
#pragma omp parallel          // Create parallel region
#pragma omp parallel for      // Parallel loop
#pragma omp critical          // Thread-safe section
omp_set_num_threads(n)        // Set thread count
omp_get_thread_num()          // Get current thread ID
omp_get_num_threads()         // Get total threads
omp_get_wtime()               // High-precision timer
```

### Scheduling Types
- **Static**: `schedule(static)` - Work divided evenly at compile time
- **Dynamic**: `schedule(dynamic, chunk_size)` - Work assigned at runtime
- **Static** = Lower overhead, good for uniform work
- **Dynamic** = Better load balancing, higher overhead

### Performance Metrics
- **Speedup** = T_serial / T_parallel
- **Efficiency** = Speedup / Number_of_threads × 100%
- **Scalability** = How speedup changes with more threads

## Expected Viva Questions & Answers

### Q: Why is outer loop parallelization better than inner loop?
**A:** Outer loop parallelization has:
- Lower synchronization overhead (creates parallel region once)
- Better cache locality (each thread works on contiguous memory)
- More work per thread (reduces thread management overhead)
- Inner loop creates/destroys parallel regions repeatedly

### Q: When would you use dynamic vs static scheduling?
**A:** 
- **Static**: Uniform workloads (like vector addition), lower overhead
- **Dynamic**: Irregular workloads with varying computation time
- **Example**: Image processing where some pixels need more computation

### Q: What is Amdahl's Law and how does it apply here?
**A:** Amdahl's Law states that speedup is limited by the sequential portion of code. As threads increase, efficiency typically decreases because:
- Thread creation/synchronization overhead increases
- Cache contention increases
- Sequential bottlenecks become more prominent

### Q: Why do small matrices show poor speedup?
**A:** 
- Thread creation overhead exceeds computation benefit
- Cache effects are minimal with small data
- Memory bandwidth not a bottleneck
- More time spent managing threads than computing

### Q: How does cache locality affect performance?
**A:**
- Each thread working on contiguous memory regions is faster
- Outer loop parallelization keeps related data together
- False sharing can hurt performance when threads modify nearby memory
- Matrix traversal patterns matter (row-major vs column-major)

### Q: What happens if you use more threads than CPU cores?
**A:**
- Context switching overhead increases
- Performance may actually decrease
- Operating system must time-slice threads
- Best practice: threads ≤ available cores

## Program-Specific Points

### Part A Key Points
- Demonstrates basic thread creation and identification
- Shows three ways to control thread count
- Environment variables vs function calls for thread control
- Thread safety in output (may see interleaved output)

### Part B Key Points
- Work-sharing construct: `#pragma omp parallel for`
- Schedule clause comparison
- Vector addition is "embarrassingly parallel"
- Static scheduling should outperform dynamic for this uniform workload

### Part C Key Points
- Matrix multiplication is O(n³) complexity
- Outer loop: each thread handles complete rows
- Inner loop: threads share row computation
- Performance tables show speedup trends
- Efficiency calculations demonstrate overhead effects

## Technical Details to Remember

### Memory Management
```c
// Proper matrix allocation
double **matrix = (double**)malloc(rows * sizeof(double*));
for (int i = 0; i < rows; i++) {
    matrix[i] = (double*)malloc(cols * sizeof(double));
}
```

### Timing Code
```c
double start = omp_get_wtime();
// ... parallel code ...
double time = omp_get_wtime() - start;
```

### Thread-Safe Output
```c
#pragma omp critical
{
    printf("Thread %d: message\n", omp_get_thread_num());
}
```

## Before Your Viva

### 1. Compilation (Choose One Method)
```bash
# Linux/Mac with GCC
make all

# Windows with batch file
compile_windows.bat

# Manual compilation (any system with GCC)
gcc -fopenmp -O2 -Wall -o program program.c -lm
```

### 2. Test All Programs
- Run each program and verify output
- Take screenshots with your machine name visible
- Note the timing results for discussion

### 3. Understand Your Results
- Which scheduling method was faster in Part B?
- What speedup did you achieve in Part C?
- Why did efficiency decrease with more threads?

## Common Pitfalls to Avoid

❌ Don't say "OpenMP automatically makes everything faster"
✅ Do explain the overhead vs benefit trade-off

❌ Don't claim inner loop parallelization is always bad  
✅ Do explain why it's inefficient for matrix multiplication specifically

❌ Don't ignore cache effects
✅ Do mention memory access patterns and locality

❌ Don't forget about thread overhead
✅ Do explain why small problems don't benefit from parallelization

## Last-Minute Checklist

- [ ] All three programs compile successfully
- [ ] You can explain the difference between static and dynamic scheduling
- [ ] You understand why outer loop parallelization performs better
- [ ] You can interpret speedup and efficiency numbers
- [ ] You have screenshots ready for submission
- [ ] You understand the relationship between problem size and parallelization benefits

## Quick Confidence Boost

You have:
- ✅ Complete, working OpenMP implementations
- ✅ Comprehensive performance analysis
- ✅ Proper commenting and code structure
- ✅ All required functionality implemented
- ✅ Ready-to-compile code with build instructions

**You're well-prepared for your viva! Good luck!** 🍀 