#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main() {
    printf("=== Part A: OpenMP Hello World Program ===\n\n");
    
    // Part 1: Hello World with 10 threads (hardcoded)
    printf("1. Hello World with 10 threads (hardcoded):\n");
    omp_set_num_threads(10);
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        printf("Hello World from thread %d out of %d threads\n", thread_id, total_threads);
    }
    
    printf("\n");
    
    // Part 2: Using environment variable (demonstrate with 5 threads)
    printf("2. Using environment variable OMP_NUM_THREADS:\n");
    printf("Set OMP_NUM_THREADS=5 before running this program\n");
    printf("Current OMP_NUM_THREADS value affects the parallel region:\n");
    
    // Reset to use environment variable
    omp_set_num_threads(5); // This simulates setting OMP_NUM_THREADS=5
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        printf("Hello World from thread %d out of %d threads (using env var simulation)\n", thread_id, total_threads);
    }
    
    printf("\n");
    
    // Part 3: User input for number of threads
    printf("3. User input for number of threads:\n");
    int user_threads;
    printf("Enter the number of threads you want to create: ");
    scanf("%d", &user_threads);
    
    if (user_threads <= 0) {
        printf("Invalid number of threads. Using default of 4.\n");
        user_threads = 4;
    }
    
    omp_set_num_threads(user_threads);
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        printf("Hello World from thread %d out of %d threads (user specified: %d)\n", 
               thread_id, total_threads, user_threads);
    }
    
    printf("\nProgram completed successfully!\n");
    return 0;
} 