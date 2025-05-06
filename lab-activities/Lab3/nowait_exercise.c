#include <stdio.h>
#include <stdlib.h> // For malloc and free
#include <omp.h>
#include <math.h>
#include <time.h>   // For srand

// Helper function to print arrays
void print_array(const char* name, float* arr, int size) {
    printf("%s: [", name);
    for (int i = 0; i < size; ++i) {
        printf("%.2f", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

// Version WITH nowait (original logic)
void nowait_version(int n, int m, float *a, float *b, float *y) {
    int i;
    printf("\n--- Running NOWAIT version ---\n");
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        #pragma omp for nowait
        for (i = 1; i < n; i++) {
            b[i] = (a[i] + a[i - 1]) / 2.0;
            printf("[NOWAIT] Thread %d processing b[%d]\n", tid, i);
        }
        // Without 'nowait', threads would wait here until ALL finished the first loop.
        // With 'nowait', threads finishing the first loop can immediately start the second.

        #pragma omp for nowait // Implicit barrier removed here too
        for (i = 0; i < m; i++) {
            // Ensure we don't read out of bounds if m > n
            if (i < n) { // Check bounds for a[i]
               y[i] = sqrtf(a[i]); // Use sqrtf for float
               printf("[NOWAIT] Thread %d processing y[%d]\n", tid, i);
            } else {
                 printf("[NOWAIT] Thread %d skipping y[%d] due to boundary check (m > n)\n", tid, i);
            }

        }
        // Implicit barrier at the end of the parallel region ensures all threads finish
        // both loops (or parts they were assigned) before the function returns.
         printf("[NOWAIT] Thread %d finished assigned work in parallel region.\n", tid);
    } // Implicit Barrier Here
    printf("--- Finished NOWAIT version ---\n");
}

// Version WITHOUT nowait (implicit barriers active)
void wait_version(int n, int m, float *a, float *b, float *y) {
    int i;
     printf("\n--- Running WAIT version ---\n");
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        #pragma omp for // Implicit barrier AT THE END of this loop
        for (i = 1; i < n; i++) {
            b[i] = (a[i] + a[i - 1]) / 2.0;
            printf("[WAIT] Thread %d processing b[%d]\n", tid, i);
        }
        // <<< Implicit barrier here: All threads MUST finish the loop above
        // before ANY thread can start the loop below.
        printf("[WAIT] Thread %d passed first barrier.\n", tid); // Will print N_THREADS times

        #pragma omp for // Implicit barrier AT THE END of this loop
        for (i = 0; i < m; i++) {
             // Ensure we don't read out of bounds if m > n
            if (i < n) { // Check bounds for a[i]
                y[i] = sqrtf(a[i]); // Use sqrtf for float
                printf("[WAIT] Thread %d processing y[%d]\n", tid, i);
            } else {
                printf("[WAIT] Thread %d skipping y[%d] due to boundary check (m > n)\n", tid, i);
            }
        }
        // <<< Implicit barrier here.
         printf("[WAIT] Thread %d finished assigned work in parallel region.\n", tid);
    } // Implicit Barrier Here
    printf("--- Finished WAIT version ---\n");
}


int main(void) {
    int n = 10; // Size for array 'a' (used up to n-1) and 'b' (size n)
    int m = 12; // Size for array 'y' (loop 2 goes up to m-1). Set m > n to test boundary.

    // Array 'a' needs to be large enough for index 'i' in loop 1 (up to n-1)
    // and index 'i' in loop 2 (up to m-1). Also needs a[i] and a[i-1] for loop 1.
    // So a must be at least size n for loop 1.
    // And at least size m for loop 2 if we are reading a[i].
    // Let's make 'a' large enough for both access patterns. Max index is max(n-1, m-1).
    // Size = max(n, m)
    int a_size = (n > m) ? n : m;

    float *a = (float*)malloc(a_size * sizeof(float));
    // b needs size n because loop 1 writes up to b[n-1]
    float *b = (float*)malloc(n * sizeof(float));
    // y needs size m because loop 2 writes up to y[m-1]
    float *y = (float*)malloc(m * sizeof(float));

    if (!a || !b || !y) {
        perror("Failed to allocate memory");
        // Clean up any successfully allocated memory before exiting
        free(a); // free(NULL) is safe
        free(b);
        free(y);
        return 1;
    }

    // Initialize array 'a' with some positive values (for sqrt)
    // Make sure values are >= 0 for sqrtf
    srand(time(NULL)); // Seed random number generator
    printf("Initializing array 'a' (size %d)...\n", a_size);
    for (int i = 0; i < a_size; ++i) {
        // Generate values between 0.0 and 99.0
        a[i] = (float)(rand() % 1000) / 10.0f;
    }
    // Initialize b and y to zero (good practice)
    for(int i=0; i<n; ++i) b[i] = 0.0f;
    for(int i=0; i<m; ++i) y[i] = 0.0f;


    print_array("Initial a", a, a_size);
    printf("\nSetting number of threads (if possible)...\n");
    // Set thread count, e.g., 4. You can change this or use environment variable OMP_NUM_THREADS
    omp_set_num_threads(4);

    // --- Run WAIT version ---
    printf("\nRunning WAIT version with n=%d, m=%d\n", n, m);
    print_array("Initial b", b, n);
    print_array("Initial y", y, m);
    wait_version(n, m, a, b, y);
    print_array("Final b (Wait)", b, n); // Note: b[0] remains 0.0
    print_array("Final y (Wait)", y, m);

     // Re-initialize b and y before running the next version
    for(int i=0; i<n; ++i) b[i] = 0.0f;
    for(int i=0; i<m; ++i) y[i] = 0.0f;
    printf("\nRe-initialized b and y.\n");


    // --- Run NOWAIT version ---
    printf("\nRunning NOWAIT version with n=%d, m=%d\n", n, m);
    print_array("Initial b", b, n);
    print_array("Initial y", y, m);
    nowait_version(n, m, a, b, y);
    print_array("Final b (NoWait)", b, n); // Note: b[0] remains 0.0
    print_array("Final y (NoWait)", y, m);


    // Free allocated memory
    free(a);
    free(b);
    free(y);

    printf("\nExecution finished.\n");
    return 0;
} 