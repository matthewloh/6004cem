# Week 2f Lab Activity -Implicit and Explicit Barrier And the NOWAIT Clause

## Compare the results of the following codes:

```c
#include <stdio.h>
#include <omp.h>

int main(void){
  #pragma omp parallel
  {
    printf("Thread %d prints 1\n", omp_get_thread_num());
    printf("Thread %d prints 2\n", omp_get_thread_num());
  }
  return 0;
}
```

Edit the previous code as follows:

```c
#include <stdio.h>
#include <omp.h>

int main(void){
  #pragma omp parallel
  {
    printf("Thread %d prints 1\n", omp_get_thread_num());
    #pragma omp barrier
    printf("Thread %d prints 2\n", omp_get_thread_num());
  }
  return 0;
}
```

For the following example add the output statements as well as the array content in order to trace the behaviour of the NOWAIT Clause. Write 2 versions of the code (one with NOWAIT and another without it) and analyse the results:

```c
#include <stdio.h>
#include <omp.h>
#include <math.h>

void nowait_example(int n, int m, float *a, float *b, float *y, float *z){
  int i;

  #pragma omp parallel
  {
    #pragma omp for nowait
    for (i = 1; i < n; i++){
      b[i] = (a[i] + a[i-1]) / 2.0;
    }
    #pragma omp for nowait
    for (i = 0; i < m; i++){
      y[i] = sqrt(a[i]);
    }

  }
}
```

---

### Analysis of the `NOWAIT` Clause Exercise

```c
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
            y[i] = sqrtf(a[i]); // Use sqrtf for float
             printf("[NOWAIT] Thread %d processing y[%d]\n", tid, i);
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
            y[i] = sqrtf(a[i]); // Use sqrtf for float
             printf("[WAIT] Thread %d processing y[%d]\n", tid, i);
        }
        // <<< Implicit barrier here.
         printf("[WAIT] Thread %d finished assigned work in parallel region.\n", tid);
    } // Implicit Barrier Here
    printf("--- Finished WAIT version ---\n");
}


int main(void) {
    int n = 10; // Size for array 'a' and 'b' (loop 1 goes up to n-1)
    int m = 10; // Size for array 'y' (loop 2 goes up to m-1)

    // Ensure 'a' is large enough for both loops (max(n, m))
    int a_size = (n > m) ? n : m;

    float *a = (float*)malloc(a_size * sizeof(float));
    float *b = (float*)malloc(n * sizeof(float)); // Note: b[0] is never written
    float *y = (float*)malloc(m * sizeof(float));
    // The original function signature included 'z', but it wasn't used.
    // float *z = (float*)malloc(m * sizeof(float));

    if (!a || !b || !y) {
        perror("Failed to allocate memory");
        return 1;
    }

    // Initialize array 'a' with some positive values (for sqrt)
    srand(time(NULL)); // Seed random number generator
    printf("Initializing array 'a' (size %d)...\n", a_size);
    for (int i = 0; i < a_size; ++i) {
        a[i] = (float)(rand() % 100) + 1.0f; // Random float between 1.0 and 100.0
    }
    // Initialize b and y (optional, but good practice)
    for(int i=0; i<n; ++i) b[i] = 0.0f;
    for(int i=0; i<m; ++i) y[i] = 0.0f;


    print_array("Initial a", a, a_size);
    printf("\nSetting number of threads (if possible)...\n");
    omp_set_num_threads(4); // Example: Use 4 threads

    // --- Run WAIT version ---
    print_array("Initial b", b, n);
    print_array("Initial y", y, m);
    wait_version(n, m, a, b, y);
    print_array("Final b (Wait)", b, n); // Note: b[0] remains uninitialized (or 0.0)
    print_array("Final y (Wait)", y, m);

     // Re-initialize b and y before running the next version
    for(int i=0; i<n; ++i) b[i] = 0.0f;
    for(int i=0; i<m; ++i) y[i] = 0.0f;
    printf("\nRe-initialized b and y.\n");


    // --- Run NOWAIT version ---
     print_array("Initial b", b, n);
    print_array("Initial y", y, m);
    nowait_version(n, m, a, b, y);
    print_array("Final b (NoWait)", b, n);
    print_array("Final y (NoWait)", y, m);


    // Free allocated memory
    free(a);
    free(b);
    free(y);
    // free(z);

    return 0;
}
```

### Explanation and Analysis

1.  **Default Behavior (Implicit Barriers - `wait_version`):**

    - When you have multiple work-sharing constructs (like `#pragma omp for`) inside a `parallel` region, there's an implicit barrier at the end of _each_ one by default.
    - In `wait_version`:
      - All threads work on the first `for` loop (calculating `b`).
      - _No thread_ can start the second `for` loop (calculating `y`) until _all threads_ have finished their share of the first loop. You'll see all "[WAIT] Thread X processing b[i]" messages appear (though interleaved among threads), followed by N_THREADS messages "[WAIT] Thread X passed first barrier.", before any "[WAIT] Thread X processing y[i]" messages appear.
      - Similarly, there's another implicit barrier after the second loop before the end of the `parallel` region.
    - This ensures that the first loop fully completes before the second loop begins.

2.  **Using `nowait` (`nowait_version`):**

    - The `nowait` clause _removes_ the implicit barrier at the end of the work-sharing construct it's attached to.
    - In `nowait_version`:
      - Threads work on the first `for` loop (calculating `b`).
      - As soon as a thread finishes its assigned iterations in the first loop, it can _immediately_ move on to grab iterations from the second `for` loop (calculating `y`), _without_ waiting for other threads to finish the first loop.
      - The second `for` loop also has `nowait`, meaning threads finishing it don't wait at its end either (though they still hit the implicit barrier at the very end of the `parallel` region).
    - **Output Observation:** You will likely see the "[NOWAIT] Thread X processing b[i]" and "[NOWAIT] Thread X processing y[i]" messages heavily interleaved. A thread might finish its work on `b` quickly and start working on `y` while other threads are still calculating `b`. The "[NOWAIT] Thread X finished..." message confirms when a thread has no more work from _either_ loop assigned to it within the parallel block.

3.  **When to Use `nowait`:**
    - `nowait` is beneficial when consecutive loops or sections within a parallel region are independent (the calculations in one do not depend on the results of the other). In this example, calculating `b` uses `a`, and calculating `y` _also_ uses `a`. They don't depend on each other. Using `nowait` allows for better load balancing and potentially faster execution by overlapping the work and reducing synchronization overhead (waiting time at barriers).
    - **Caution:** If the second loop _depended_ on the results calculated in the first loop (e.g., if the second loop used array `b`), adding `nowait` to the first loop would create a _race condition_. Threads might start the second loop and read values from `b` before those values have been correctly computed by other threads still working on the first loop, leading to incorrect results. In such dependent cases, the implicit barrier (or an explicit `#pragma omp barrier`) is crucial for correctness.

By running both versions and observing the print statements and final array contents (which should be identical in both versions for _this specific independent example_), you can clearly see the difference in execution flow caused by the presence or absence of the barrier synchronization. The `nowait` version allows threads to proceed more freely when tasks are independent.
