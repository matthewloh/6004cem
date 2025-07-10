#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Function to initialize a matrix with random values
void initialize_matrix(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX * 10.0;
        }
    }
}

// Function to allocate memory for a matrix
double** allocate_matrix(int rows, int cols) {
    double **matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(cols * sizeof(double));
    }
    return matrix;
}

// Function to free matrix memory
void free_matrix(double **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Matrix multiplication with outer loop parallelization
double matrix_multiply_outer_parallel(double **A, double **B, double **C, int size, int num_threads) {
    omp_set_num_threads(num_threads);
    double start_time = omp_get_wtime();
    
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    double end_time = omp_get_wtime();
    return end_time - start_time;
}

// Matrix multiplication with inner loop parallelization
double matrix_multiply_inner_parallel(double **A, double **B, double **C, int size, int num_threads) {
    omp_set_num_threads(num_threads);
    double start_time = omp_get_wtime();
    
    for (int i = 0; i < size; i++) {
        #pragma omp parallel for
        for (int j = 0; j < size; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    double end_time = omp_get_wtime();
    return end_time - start_time;
}

// Function to run timing tests
void run_timing_tests(int matrix_size, const char* size_label) {
    printf("\n=== Matrix Size: %s (%dx%d) ===\n", size_label, matrix_size, matrix_size);
    
    // Allocate matrices
    double **A = allocate_matrix(matrix_size, matrix_size);
    double **B = allocate_matrix(matrix_size, matrix_size);
    double **C = allocate_matrix(matrix_size, matrix_size);
    
    // Initialize matrices
    srand(42); // Fixed seed for reproducible results
    initialize_matrix(A, matrix_size, matrix_size);
    initialize_matrix(B, matrix_size, matrix_size);
    
    int thread_counts[] = {1, 4, 8, 16};
    int num_thread_configs = 4;
    int num_runs = 10;
    
    printf("\nOuter Loop Parallelization Results:\n");
    printf("Threads\t| ");
    for (int run = 0; run < num_runs; run++) {
        printf("Run %d\t\t", run + 1);
    }
    printf("| Average\n");
    printf("--------|");
    for (int run = 0; run < num_runs; run++) {
        printf("----------");
    }
    printf("|----------\n");
    
    double outer_averages[4];
    
    for (int t = 0; t < num_thread_configs; t++) {
        int threads = thread_counts[t];
        printf("%d\t| ", threads);
        
        double total_time = 0.0;
        for (int run = 0; run < num_runs; run++) {
            double time = matrix_multiply_outer_parallel(A, B, C, matrix_size, threads);
            printf("%.6f\t", time);
            total_time += time;
        }
        
        double average = total_time / num_runs;
        outer_averages[t] = average;
        printf("| %.6f\n", average);
    }
    
    printf("\nInner Loop Parallelization Results:\n");
    printf("Threads\t| ");
    for (int run = 0; run < num_runs; run++) {
        printf("Run %d\t\t", run + 1);
    }
    printf("| Average\n");
    printf("--------|");
    for (int run = 0; run < num_runs; run++) {
        printf("----------");
    }
    printf("|----------\n");
    
    double inner_averages[4];
    
    for (int t = 0; t < num_thread_configs; t++) {
        int threads = thread_counts[t];
        printf("%d\t| ", threads);
        
        double total_time = 0.0;
        for (int run = 0; run < num_runs; run++) {
            double time = matrix_multiply_inner_parallel(A, B, C, matrix_size, threads);
            printf("%.6f\t", time);
            total_time += time;
        }
        
        double average = total_time / num_runs;
        inner_averages[t] = average;
        printf("| %.6f\n", average);
    }
    
    // Performance analysis
    printf("\n=== Performance Analysis for %s ===\n", size_label);
    printf("Speedup Analysis (compared to 1 thread):\n");
    printf("Threads\t| Outer Loop Speedup\t| Inner Loop Speedup\n");
    printf("--------|----------------------|----------------------\n");
    
    for (int t = 0; t < num_thread_configs; t++) {
        double outer_speedup = outer_averages[0] / outer_averages[t];
        double inner_speedup = inner_averages[0] / inner_averages[t];
        printf("%d\t| %.2fx\t\t\t| %.2fx\n", thread_counts[t], outer_speedup, inner_speedup);
    }
    
    printf("\nEfficiency Analysis:\n");
    printf("Threads\t| Outer Loop Efficiency\t| Inner Loop Efficiency\n");
    printf("--------|----------------------|----------------------\n");
    
    for (int t = 0; t < num_thread_configs; t++) {
        double outer_speedup = outer_averages[0] / outer_averages[t];
        double inner_speedup = inner_averages[0] / inner_averages[t];
        double outer_efficiency = outer_speedup / thread_counts[t] * 100;
        double inner_efficiency = inner_speedup / thread_counts[t] * 100;
        printf("%d\t| %.1f%%\t\t\t| %.1f%%\n", thread_counts[t], outer_efficiency, inner_efficiency);
    }
    
    // Free matrices
    free_matrix(A, matrix_size);
    free_matrix(B, matrix_size);
    free_matrix(C, matrix_size);
}

int main() {
    printf("=== Part C: OpenMP Matrix Calculation Performance Analysis ===\n");
    printf("Testing matrix multiplication with different parallelization strategies\n");
    printf("Each test runs 10 times and results are averaged\n");
    
    // Test with 50x50 matrix
    run_timing_tests(50, "50x50");
    
    // Test with 500x500 matrix
    run_timing_tests(500, "500x500");
    
    printf("\n=== Overall Analysis and Conclusions ===\n");
    printf("\nOUTER LOOP PARALLELIZATION:\n");
    printf("- Parallelizes the outermost loop (rows)\n");
    printf("- Better load distribution for larger matrices\n");
    printf("- Lower synchronization overhead\n");
    printf("- Generally more efficient for matrix multiplication\n");
    
    printf("\nINNER LOOP PARALLELIZATION:\n");
    printf("- Parallelizes the middle loop (columns)\n");
    printf("- Higher synchronization overhead due to frequent parallel regions\n");
    printf("- May show poor performance due to overhead exceeding benefits\n");
    printf("- Less suitable for matrix multiplication\n");
    
    printf("\nKEY OBSERVATIONS:\n");
    printf("1. Outer loop parallelization typically shows better scalability\n");
    printf("2. Performance gains are more pronounced with larger matrices\n");
    printf("3. Thread overhead may limit speedup for smaller matrices\n");
    printf("4. Efficiency typically decreases as thread count increases\n");
    printf("5. Cache locality affects performance significantly\n");
    
    printf("\nRECOMMENDATIONS:\n");
    printf("- Use outer loop parallelization for matrix multiplication\n");
    printf("- Consider matrix size when choosing thread count\n");
    printf("- Monitor efficiency to find optimal thread configuration\n");
    printf("- Account for system architecture and available cores\n");
    
    printf("\nProgram completed successfully!\n");
    return 0;
} 