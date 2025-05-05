#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

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
  int threshold =
      5000; // Threshold for conditional parallelism (N*N > threshold)

  printf("Matrix size: %d x %d\n", N, N);

  // Allocate matrices
  int **matrix_a = allocate_matrix(N, N);
  int **matrix_b = allocate_matrix(N, N);
  int **result_c = allocate_matrix(N, N);

  // Initialize matrices
  initialize_matrix(matrix_a, N, N);
  initialize_matrix(matrix_b, N, N);

  double start_time, end_time;

  printf("\n--- a) Sequential Multiplication ---\n");
  start_time = omp_get_wtime();
  multiply_sequential(matrix_a, matrix_b, result_c, N, N, N);
  end_time = omp_get_wtime();
  printf("Time: %f seconds\n", end_time - start_time);
  // small N

  printf("\n--- b) Forced Parallel Multiplication ---\n");
  // Reset result matrix (or use a different one)
  initialize_matrix(
      result_c, N, N); // Re-initialize C to zeros or some known state if needed
  start_time = omp_get_wtime();
  multiply_forced_parallel(matrix_a, matrix_b, result_c, N, N, N);
  end_time = omp_get_wtime();
  printf("Time: %f seconds\n", end_time - start_time);

  printf(
      "\n--- c) Conditional Parallel Multiplication (Initial Threshold) ---\n");
  printf("Initial threshold = %d elements\n", threshold);
  initialize_matrix(result_c, N, N); // Re-initialize C
  start_time = omp_get_wtime();
  multiply_conditional_parallel(matrix_a, matrix_b, result_c, N, N, N,
                                threshold);
  end_time = omp_get_wtime();
  printf("Time: %f seconds\n", end_time - start_time);

  printf("\n--- Testing Different Thresholds for Conditional Parallelism "
         "---\n");
  int thresholds[] = {1000,   5000,   10000,  50000,
                      100000, 500000, 1000000}; // Example thresholds (N*N)
  for (int i = 0; i < sizeof(thresholds) / sizeof(thresholds[0]); ++i) {
    threshold = thresholds[i];
    if (N * N < threshold) {
      printf("Threshold %5d: Skipped (Matrix size %d < Threshold)\n", threshold,
             N * N);
      continue;
    }
    initialize_matrix(result_c, N, N); // Re-initialize C
    start_time = omp_get_wtime();
    multiply_conditional_parallel(matrix_a, matrix_b, result_c, N, N, N,
                                  threshold);
    end_time = omp_get_wtime();
    printf("Threshold %5d: Time = %f seconds (Parallel? %s)\n", threshold,
           end_time - start_time, (N * N > threshold) ? "Yes" : "No");
  }

  printf("-----------------------------------------------------\n");

  // Free allocated memory
  free_matrix(matrix_a, N);
  free_matrix(matrix_b, N);
  free_matrix(result_c, N);

  return 0;
}
