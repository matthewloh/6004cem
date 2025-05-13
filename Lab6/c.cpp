#include <omp.h>
#include <stdio.h>

int main(void) {
  int sum = 100;
  printf("Before parallelism, sum's adress is %p\n", &sum);
#pragma omp parallel for reduction(+ : sum)
  for (int i = 1; i <= 4; i++) {
    printf("sum's address in thread %d is %p, value is %d\n",
           omp_get_thread_num(), &sum, sum);
    sum += i;
  }

  printf("After parallelism, sum's address is %p, and value is %d\n", &sum,
         sum);
  return 0;
}
