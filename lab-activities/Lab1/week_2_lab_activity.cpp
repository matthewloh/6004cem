#include <iostream>
#include <omp.h>

int main() {
    // int desired_threads = 4;
    // Set the number of threads for subsequent parallel regions
      
    // omp_set_num_threads(desired_threads);
    // #pragma omp parallel
    //
    // or 
    // or (by setting num_threads next to the compiler directive)
    #pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        std::cout << "Hello World from thread " << thread_id 
                  << " of " << num_threads << std::endl;
    }
    return 0;
}

// or
// at run time, set the environment variable OMP_NUM_THREADS to 4 (example)
// g++ -fopenmp hello_omp.cpp -o hello_omp_cpp && OMP_NUM_THREADS=4 ./hello_omp_cpp 
