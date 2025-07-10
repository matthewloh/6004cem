#include <omp.h>
#include <stdio.h>

int main(void) {
#pragma omp parallel
  {
    printf("Thread %d prints 1\n", omp_get_thread_num());
    printf("Thread %d prints 2\n", omp_get_thread_num());
  }
  return 0;
}
