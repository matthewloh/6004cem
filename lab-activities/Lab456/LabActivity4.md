# Explicit Barrier

Identify what is the difference between
(i) explicit barrier and implicit barrier and
(ii) Explicit barrier and critical
(iii) Single and critical
(iv) Atomic and Single

## Single

```cpp
#include <iostream>
#include <omp.h>
int main() {
    #pragma omp parallel
    {
      // private variable
      int sum_local = 0;

      #pragma omp for
      for (int i = 0; i < 10; ++i) {
        #pragma omp critical
        {
          std::cout << "Thread " << omp_get_thread_num() << " is working on task " << i << std::endl;
        }
      }
    }
    return 0;
}
```

Set OMP_NUM_THREADS=4 and run the program.

## Atomic

```cpp
// omp_atomic.cpp
// compile with: /openmp
#include <stdio.h>
#include <omp.h>

#define MAX 10
int main() {
    int count = 0;
    int single_count = 0;

    printf("Demonstrating atomic vs single directives:\n");
    printf("----------------------------------------\n");

    #pragma omp parallel num_threads(MAX)
    {
        // Atomic operation - each thread can execute this
        #pragma omp atomic
        count++;
        
        // Single operation - only one thread will execute this
        #pragma omp single
        {
            single_count++;
            printf("Thread %d executed the single region\n", omp_get_thread_num());
        }
        
        // Print which thread is executing
        printf("Thread %d: count = %d, single_count = %d\n", 
               omp_get_thread_num(), count, single_count);
    }

    printf("\nFinal results:\n");
    printf("Atomic count: %d (should be %d)\n", count, MAX);
    printf("Single count: %d (should be 1)\n", single_count);
    return 0;
}
```

## Differences Between OpenMP Directives

### (i) Explicit Barrier vs Implicit Barrier
- Explicit Barrier: Created using `#pragma omp barrier`. All threads must reach this point before any can proceed.
- Implicit Barrier: Automatically added at the end of parallel regions, for, single, and sections constructs unless nowait is specified.
- Key difference: Explicit barriers give more control over synchronization points, while implicit barriers are automatically added for safety.

### (ii) Explicit Barrier vs Critical
- Explicit Barrier**: Synchronizes all threads at a specific point, forcing them to wait for each other.
- **Critical**: Creates a mutual exclusion region where only one thread can execute at a time.
- Key difference: Barriers synchronize all threads, while critical sections only ensure one thread executes at a time without forcing all threads to wait.

### (iii) Single vs Critical
- **Single**: Ensures a code block is executed by exactly one thread (chosen by the runtime), with other threads waiting at an implicit barrier.
- **Critical**: Allows any thread to execute the code block, but ensures only one thread executes it at a time.
- Key difference: Single is about execution by one specific thread, while critical is about mutual exclusion for any thread.

### (iv) Atomic vs Single
- **Atomic**: Ensures a single memory operation (like increment) is performed atomically by any thread.
- **Single**: Ensures a block of code is executed by exactly one thread.
- Key differences:
  - Atomic is for simple operations (like ++, +=, etc.), while single can contain complex code blocks
  - Atomic allows multiple threads to perform the operation (just not simultaneously), while single ensures only one thread ever executes the code
  - Atomic is more efficient for simple operations, while single is more flexible for complex code blocks

## Example Results
When running the modified atomic.cpp program, you'll see:
1. The atomic counter (`count`) will reach 10 because all threads can increment it
2. The single counter (`single_count`) will only reach 1 because only one thread executes that section
3. The output will show which thread executed the single region
4. Each thread will print its view of both counters, demonstrating the different behaviors

This demonstrates how these directives serve different purposes in parallel programming:
- Use `atomic` for thread-safe simple operations
- Use `single` when you need exactly one thread to execute a code block
- Use `critical` when you need mutual exclusion for any thread
- Use `barrier` when you need to synchronize all threads at a specific point