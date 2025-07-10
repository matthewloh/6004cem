#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int i;
  int x;
  x = 44;
#pragma omp parallel for firstprivate(x)
  for (i = 0; i <= 10; i++) {
    x = x + i;
    printf("Thread number: %d x: %d\n", omp_get_thread_num(), x);
  }
  printf("x is %d\n", x);
  return 0;
}
