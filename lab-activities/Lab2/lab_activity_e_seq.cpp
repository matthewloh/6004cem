#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Needed for strcmp

// Function to allocate a matrix
int **allocate_matrix(int rows, int cols) {
  int **matrix = (int **)malloc(rows * sizeof(int *));
  if (matrix == NULL) {
    perror("Failed to allocate memory for matrix rows");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < rows; i++) {
    matrix[i] = (int *)malloc(cols * sizeof(int));
    if (matrix[i] == NULL) {
      perror("Failed to allocate memory for matrix columns");
      // Free previously allocated rows before exiting
      for (int j = 0; j < i; j++) {
        free(matrix[j]);
      }
      free(matrix);
      exit(EXIT_FAILURE);
    }
  }
  return matrix;
}

// Function to free a matrix
void free_matrix(int **matrix, int rows) {
  if (matrix == NULL)
    return;
  for (int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

// Function to initialize a matrix with simple values
void initialize_matrix(int **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      matrix[i][j] = i + j; // Simple initialization
    }
  }
}

// a) Fully sequential matrix multiplication
void multiply_sequential(int **a, int **b, int **c, int rows, int cols,
                         int inner_dim) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      c[i][j] = 0;
      for (int k = 0; k < inner_dim; k++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

// b) Forced Parallelism matrix multiplication
void multiply_forced_parallel(int **a, int **b, int **c, int rows, int cols,
                              int inner_dim) {
#pragma omp parallel for collapse(2) // Parallelize the outer two loops
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      c[i][j] = 0; // Initialize element C[i][j]
      for (int k = 0; k < inner_dim; k++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

// c) Parallelism with using if clause
void multiply_conditional_parallel(int **a, int **b, int **c, int rows,
                                   int cols, int inner_dim, int threshold) {
#pragma omp parallel for collapse(                                             \
        2) if (rows * cols > threshold) // Conditional parallelism
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      c[i][j] = 0; // Initialize element C[i][j]
      for (int k = 0; k < inner_dim; k++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

int main() {
  int N = 1000; // Matrix size (rows and columns). Change to 1000 as required.
  int initial_threshold = 5000; // Initial threshold for conditional parallelism
  const int NUM_TRIALS = 10;

  // Print CSV header
  printf("Method,Trial,Size,Threshold,Time\n");

  // Allocate matrices once outside the trial loop
  int **matrix_a = allocate_matrix(N, N);
  int **matrix_b = allocate_matrix(N, N);
  int **result_c = allocate_matrix(N, N);

  // Initialize matrices once
  initialize_matrix(matrix_a, N, N);
  initialize_matrix(matrix_b, N, N);

  for (int trial = 0; trial < NUM_TRIALS; ++trial) {
    double start_time, end_time;

    // --- a) Sequential Multiplication ---
    // No need to re-initialize result_c if it's overwritten anyway
    start_time = omp_get_wtime();
    multiply_sequential(matrix_a, matrix_b, result_c, N, N, N);
    end_time = omp_get_wtime();
    printf("Sequential,%d,%d,%d,%f\n", trial, N, 0, end_time - start_time);

    // --- b) Forced Parallel Multiplication ---
    start_time = omp_get_wtime();
    multiply_forced_parallel(matrix_a, matrix_b, result_c, N, N, N);
    end_time = omp_get_wtime();
    printf("ForcedParallel,%d,%d,%d,%f\n", trial, N, 0, end_time - start_time);

    // --- c) Conditional Parallel Multiplication (Initial Threshold) ---
    start_time = omp_get_wtime();
    multiply_conditional_parallel(matrix_a, matrix_b, result_c, N, N, N,
                                  initial_threshold);
    end_time = omp_get_wtime();
    printf("ConditionalParallelInitial,%d,%d,%d,%f\n", trial, N,
           initial_threshold, end_time - start_time);

    // --- Testing Different Thresholds for Conditional Parallelism ---
    int thresholds[] = {1000,   5000,   10000,  50000,
                        100000, 500000, 1000000}; // Example thresholds (N*N)
    int num_thresholds = sizeof(thresholds) / sizeof(thresholds[0]);

    for (int i = 0; i < num_thresholds; ++i) {
      int current_threshold = thresholds[i];
      // No need to check N*N < threshold here, just run and record time.
      // The if condition is inside the multiply function itself.
      start_time = omp_get_wtime();
      multiply_conditional_parallel(matrix_a, matrix_b, result_c, N, N, N,
                                    current_threshold);
      end_time = omp_get_wtime();
      // Distinguish these results from the initial threshold run
      printf("ConditionalParallelThreshold,%d,%d,%d,%f\n", trial, N,
             current_threshold, end_time - start_time);
    }
  } // End of trial loop

  // Free allocated memory once after all trials
  free_matrix(matrix_a, N);
  free_matrix(matrix_b, N);
  free_matrix(result_c, N);

  return 0;
}
