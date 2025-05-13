#include <omp.h>
#include <stdio.h>

int main(void) {
  int sum = 100;
#pragma omp parallel for
  for (int i = 1; i <= 4; i++) {
    sum += i;
  }

  printf("sum is %d\n", sum);
  return 0;
}
