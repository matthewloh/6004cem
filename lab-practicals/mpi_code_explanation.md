# Detailed Explanation of MPI Code (Example B)

## Code Structure Analysis:

### 1. **Header and Variable Declaration**

```cpp
#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, i;
    int array[100];
    int local_sum = 0, total_sum = 0;
```

- `rank`: Process ID (0, 1, 2, ... n-1)
- `size`: Total number of processes
- `array[100]`: The data to be processed (only used by rank 0)
- `local_sum`: Each process's partial sum
- `total_sum`: Final result (only meaningful at rank 0)

### 2. **MPI Initialization**

```cpp
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
```

- `MPI_Init()`: Initializes MPI environment
- `MPI_Comm_rank()`: Gets current process's rank
- `MPI_Comm_size()`: Gets total number of processes
- `MPI_COMM_WORLD`: Default communicator (all processes)

### 3. **Data Initialization (Root Process Only)**

```cpp
if (rank == 0) {
    for (i = 0; i < 100; i++) array[i] = i + 1;
}
```

- Only process 0 (root) initializes the array
- Array contains values 1, 2, 3, ..., 100
- Other processes have uninitialized arrays (doesn't matter)

### 4. **Data Distribution (Scatter)**

```cpp
int chunk = 100 / size;
int local_array[chunk];
MPI_Scatter(array, chunk, MPI_INT, local_array, chunk, MPI_INT, 0, MPI_COMM_WORLD);
```

- `chunk`: How many elements each process gets
- `local_array`: Each process's portion of data
- `MPI_Scatter()`: Distributes data from root to all processes
  - Source: `array` (at root process 0)
  - Each process receives `chunk` elements
  - Stored in each process's `local_array`

**Example with 4 processes:**

- Process 0 gets elements 1-25
- Process 1 gets elements 26-50
- Process 2 gets elements 51-75
- Process 3 gets elements 76-100

### 5. **Parallel Computation**

```cpp
for (i = 0; i < chunk; i++) {
    local_sum += local_array[i];
}
```

- Each process computes sum of its local data
- This happens simultaneously on all processes
- No communication needed during computation

### 6. **Result Collection (Reduce)**

```cpp
MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
```

- `MPI_Reduce()`: Combines results from all processes
- Operation: `MPI_SUM` (adds all local_sum values)
- Result stored in `total_sum` at process 0
- Other processes' `total_sum` remains unchanged

### 7. **Output and Cleanup**

```cpp
if (rank == 0) {
    printf("Total sum (MPI): %d\n", total_sum);
}
MPI_Finalize();
```

- Only root process prints the result
- `MPI_Finalize()`: Cleans up MPI environment

## **Expected Output:**

```
Total sum (MPI): 5050
```

## **Process Flow Diagram:**

```
Process 0: [1,2,3,...,25]  → local_sum = 325
Process 1: [26,27,...,50]  → local_sum = 950
Process 2: [51,52,...,75]  → local_sum = 1575
Process 3: [76,77,...,100] → local_sum = 2200

MPI_Reduce with MPI_SUM:
total_sum = 325 + 950 + 1575 + 2200 = 5050
```

## **Key MPI Concepts Demonstrated:**

1. **SPMD Model**: Same program runs on all processes
2. **Data Distribution**: Scatter operation
3. **Parallel Processing**: Independent computation
4. **Result Aggregation**: Reduce operation
5. **Process Coordination**: Root process coordination
