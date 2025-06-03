#include <omp.h>
#include <stdio.h>

int main() {
  // Example 1: Basic Reduction
  printf("\nExample 1: Basic Reduction\n");
  int sum = 0;
#pragma omp parallel for reduction(+ : sum)
  for (int i = 0; i < 10; i++) {
    sum += i;
  }
  printf("sum = %d\n", sum); // Should print 45 (sum of 0 to 9)

  // Example 2: Multiple Reduction Clauses
  printf("\nExample 2: Multiple Reduction Clauses\n");
  int sa = 0, sb = 0, m = 0;
  int A[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int B[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

#pragma omp parallel for reduction(+ : sa, sb) reduction(max : m)
  for (int i = 0; i < 10; i++) {
    sa += A[i];
    sb += B[i];
    if (A[i] > m) {
      m = A[i];
    }
    m = B[i] > m ? B[i] : m;
  }
  printf("sa = %d, sb = %d, m = %d\n", sa, sb, m);

  // Example 3: Shared vs Private without Reduction
  printf("\nExample 3: Shared vs Private without Reduction\n");
  int x[10000];
  for (int i = 0; i < 10000; i++) {
    x[i] = 1;
  }
  sum = 0;
  int i; // Declare i outside the parallel region
#pragma omp parallel
  {
#pragma omp for
    for (i = 0; i < 10000; i++) {
      sum += x[i];
    }
  }
  printf("sum (without reduction) = %d\n", sum);

  // Example 4: Shared vs Private with Reduction
  printf("\nExample 4: Shared vs Private with Reduction\n");
  sum = 0;
#pragma omp parallel
  {
#pragma omp for reduction(+ : sum)
    for (i = 0; i < 10000; i++) {
      sum += x[i];
    }
  }
  printf("sum (with reduction) = %d\n", sum);

  // Example 5: Thread Identification with lastprivate
  printf("\nExample 5: Thread Identification with lastprivate\n");
  int k = 0;
#pragma omp parallel
  {
    int thread_id = omp_get_thread_num();
#pragma omp for lastprivate(k)
    for (i = 0; i < 10; i++) {
      k = i * i;
      printf("Thread %d processing i = %d, k = %d\n", thread_id, i, k);
    }
  }
  printf("Final k = %d\n", k);

  return 0;
}