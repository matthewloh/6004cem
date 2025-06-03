#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define VECTOR_SIZE 1000000

void initialize_vectors(double *a, double *b, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = (double)rand() / RAND_MAX * 100.0;
        b[i] = (double)rand() / RAND_MAX * 100.0;
    }
}

void print_sample_results(double *a, double *b, double *c, int size) {
    printf("Sample results (first 10 elements):\n");
    for (int i = 0; i < 10 && i < size; i++) {
        printf("c[%d] = %.2f + %.2f = %.2f\n", i, a[i], b[i], c[i]);
    }
    printf("...\n");
}

int main() {
    printf("=== Part B: Vector Addition with OpenMP Work-Sharing ===\n\n");
    
    // Allocate memory for vectors
    double *vector_a = (double*)malloc(VECTOR_SIZE * sizeof(double));
    double *vector_b = (double*)malloc(VECTOR_SIZE * sizeof(double));
    double *vector_c_static = (double*)malloc(VECTOR_SIZE * sizeof(double));
    double *vector_c_dynamic = (double*)malloc(VECTOR_SIZE * sizeof(double));
    
    if (!vector_a || !vector_b || !vector_c_static || !vector_c_dynamic) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize vectors with random values
    printf("Initializing vectors with %d elements...\n", VECTOR_SIZE);
    initialize_vectors(vector_a, vector_b, VECTOR_SIZE);
    
    // Set number of threads
    omp_set_num_threads(4);
    printf("Using %d threads for parallel computation\n\n", omp_get_max_threads());
    
    // 1. Static Scheduling
    printf("1. Vector Addition with STATIC Scheduling:\n");
    double start_time = omp_get_wtime();
    
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector_c_static[i] = vector_a[i] + vector_b[i];
        
        // Print thread info for first few iterations (for demonstration)
        if (i < 20) {
            #pragma omp critical
            {
                printf("Thread %d processing element %d\n", omp_get_thread_num(), i);
            }
        }
    }
    
    double static_time = omp_get_wtime() - start_time;
    printf("Static scheduling completed in %.6f seconds\n", static_time);
    print_sample_results(vector_a, vector_b, vector_c_static, VECTOR_SIZE);
    printf("\n");
    
    // 2. Dynamic Scheduling
    printf("2. Vector Addition with DYNAMIC Scheduling:\n");
    start_time = omp_get_wtime();
    
    #pragma omp parallel for schedule(dynamic, 1000)
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector_c_dynamic[i] = vector_a[i] + vector_b[i];
        
        // Print thread info for first few iterations (for demonstration)
        if (i < 20) {
            #pragma omp critical
            {
                printf("Thread %d processing element %d\n", omp_get_thread_num(), i);
            }
        }
    }
    
    double dynamic_time = omp_get_wtime() - start_time;
    printf("Dynamic scheduling completed in %.6f seconds\n", dynamic_time);
    print_sample_results(vector_a, vector_b, vector_c_dynamic, VECTOR_SIZE);
    printf("\n");
    
    // 3. Performance Comparison and Analysis
    printf("=== Performance Analysis ===\n");
    printf("Static scheduling time:  %.6f seconds\n", static_time);
    printf("Dynamic scheduling time: %.6f seconds\n", dynamic_time);
    printf("Performance difference: %.2f%%\n", 
           ((dynamic_time - static_time) / static_time) * 100.0);
    
    // Verify results are identical
    int results_match = 1;
    for (int i = 0; i < VECTOR_SIZE; i++) {
        if (vector_c_static[i] != vector_c_dynamic[i]) {
            results_match = 0;
            break;
        }
    }
    
    printf("Results verification: %s\n", results_match ? "PASSED - Both methods produce identical results" : "FAILED - Results differ");
    
    printf("\n=== Analysis and Conclusions ===\n");
    printf("STATIC SCHEDULING:\n");
    printf("- Divides iterations evenly among threads at compile time\n");
    printf("- Lower overhead due to no runtime load balancing\n");
    printf("- Good for uniform workloads like simple vector addition\n");
    printf("- Predictable thread assignment\n\n");
    
    printf("DYNAMIC SCHEDULING:\n");
    printf("- Assigns chunks of iterations to threads at runtime\n");
    printf("- Higher overhead due to runtime scheduling decisions\n");
    printf("- Better for irregular workloads with varying computation time\n");
    printf("- Can provide better load balancing for uneven work distribution\n\n");
    
    printf("FOR THIS PROBLEM:\n");
    printf("Vector addition has uniform computational complexity per element,\n");
    printf("so static scheduling is generally more efficient due to lower overhead.\n");
    printf("Dynamic scheduling shows its benefits when work distribution is uneven.\n");
    
    // Cleanup
    free(vector_a);
    free(vector_b);
    free(vector_c_static);
    free(vector_c_dynamic);
    
    printf("\nProgram completed successfully!\n");
    return 0;
} 