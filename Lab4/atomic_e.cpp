// omp_atomic.cpp
// compile with: /openmp
#include <omp.h>
#include <stdio.h>

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
    printf("Thread %d: count = %d, single_count = %d\n", omp_get_thread_num(),
           count, single_count);
  }

  printf("\nFinal results:\n");
  printf("Atomic count: %d (should be %d)\n", count, MAX);
  printf("Single count: %d (should be 1)\n", single_count);
  return 0;
}
