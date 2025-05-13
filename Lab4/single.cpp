#include <iostream>
#include <omp.h>
int main() {
#pragma omp parallel
  {
    // private variable
    int sum_local = 0;

#pragma omp for nowait
    for (int i = 0; i < 10; ++i) {
#pragma omp critical
      {
        std::cout << "Execution started" << std::endl;
        std::cout << "Thread " << omp_get_thread_num() << " is working on task "
                  << i << std::endl;
        std::cout << "Execution completed by Thread " << omp_get_thread_num()
                  << std::endl;
        ;
      }
      sum_local += i;
      std::cout << "Sum_local = " << sum_local << std::endl;
    }
  }
  return 0;
}
