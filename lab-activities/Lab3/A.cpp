#include "omp.h"
#include <stdio.h>
#include <stdlib.h>
int main() {
  int nthreads, tid;

  omp_set_num_threads(3);
#pragma omp parallel private(tid)
  {
    int i;
    tid = omp_get_thread_num();
    printf("Hello world from (%d)\n", tid);
#pragma omp for
    for (i = 0; i <= 4; i++) {
      printf("Iteration % d by % d\n", i, tid);
    }
  } // all threads join master thread and terminates
}
