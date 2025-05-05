#include <omp.h>
#include <stdio.h>

int main() {
  int i, n = 200;
  int sum = 0;
  int a[n];

  // Initialize array
  for (i = 0; i < n; i++) {
    a[i] = i + 1;
  }

// Sequential or parallel based on n
#pragma omp parallel for if (n > 100) reduction(+ : sum)
  for (i = 0; i < n; i++) {
    sum += a[i];
  }

  printf("Sum = %d\n", sum);
  return 0;
}
