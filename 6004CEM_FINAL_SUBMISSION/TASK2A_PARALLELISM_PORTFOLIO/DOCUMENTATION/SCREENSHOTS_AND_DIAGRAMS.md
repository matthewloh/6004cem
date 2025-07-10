# Task 2A: Parallelism Portfolio - Screenshot and Diagram Guide

This document provides a guide for generating screenshots and diagrams for the OpenMP programs in this portfolio.

---

## 📸 Part A: Enhanced OpenMP Hello World

### Recommended Diagrams

#### 1. Basic OpenMP Execution Flow

This diagram shows how a master thread forks a team of parallel threads and joins them after the parallel region is complete.

```mermaid
graph TD
    A[Start Program] --> B{Master Thread Begins};
    B --> C["#pragma omp parallel"];

    subgraph "Parallel Region"
        direction LR
        T1["Thread 0"]
        T2["Thread 1"]
        T3["Thread 2"]
        T4["..."]
        T5["Thread N-1"]
    end

    C -- Fork --> T1;
    C -- Fork --> T2;
    C -- Fork --> T3;
    C -- Fork --> T4;
    C -- Fork --> T5;

    T1 -- Join --> D{End of Parallel Region};
    T2 -- Join --> D;
    T3 -- Join --> D;
    T4 -- Join --> D;
    T5 -- Join --> D;

    D --> E{Master Thread Continues};
    E --> F[End Program];

    style C fill:#f9f,stroke:#333,stroke-width:2px
```

#### 2. Private vs. Shared Variables

This diagram illustrates the concepts of `shared` and `private` variables as seen in the `demonstrate_advanced_features` function.

```mermaid
graph TD
    subgraph "omp parallel"
        direction TB

        subgraph "Shared Memory"
            S["Shared Variable<br/>(e.g., 'sum' in reduction)"]
        end

        subgraph "Thread 1"
            P1["Private Variable<br/>(e.g., 'private_var')<br/>Value is independent"]
        end

        subgraph "Thread 2"
            P2["Private Variable<br/>(e.g., 'private_var')<br/>Value is independent"]
        end

        subgraph "Thread N"
            PN["..."]
        end

        T1_exec["Thread 1 Execution"]
        T2_exec["Thread 2 Execution"]
        TN_exec["..."]

        T1_exec --> P1;
        T2_exec --> P2;

        T1_exec -- "reads/writes" --> S;
        T2_exec -- "reads/writes" --> S;
        TN_exec -- "reads/writes" --> S;
    end

    style S fill:#cde4ff
    style P1 fill:#d4edda
    style P2 fill:#d4edda
```

---

## 📸 Part B: Enhanced Vector Addition

### Recommended Diagrams

#### 1. Static Scheduling (`schedule(static)`)

Illustrates how static scheduling divides iterations into contiguous, pre-determined chunks. This is ideal for loops where each iteration takes a similar amount of time.

```mermaid
gantt
    title Static Scheduling (12 Iterations, 4 Threads)
    dateFormat  X
    axisFormat %s

    section Thread 0
    Chunk 1: 0, 3, 3

    section Thread 1
    Chunk 2: 3, 3, 3

    section Thread 2
    Chunk 3: 6, 3, 3

    section Thread 3
    Chunk 4: 9, 3, 3
```

#### 2. Dynamic Scheduling (`schedule(dynamic, chunk_size)`)

Shows how dynamic scheduling assigns smaller chunks of iterations to threads as they become available. This is better for loops with varying iteration runtimes, as it provides better load balancing.

```mermaid
gantt
    title Dynamic Scheduling (chunk_size=1, 4 Threads)
    dateFormat X
    axisFormat %s

    section Thread 0
    Iter 0: 0, 1, 1
    Iter 4: 4, 1, 1
    Iter 8: 8, 1, 1

    section Thread 1
    Iter 1: 1, 1, 1
    Iter 5: 5, 1, 1
    Iter 9: 9, 1, 1

    section Thread 2
    Iter 2: 2, 1, 1
    Iter 6: 6, 1, 1
    Iter 10: 10, 1, 1

    section Thread 3
    Iter 3: 3, 1, 1
    Iter 7: 7, 1, 1
    Iter 11: 11, 1
```

---

## 📸 Part C: Enhanced Matrix Calculation

### Recommended Diagrams

#### 1. Outer Loop Parallelization

This is the more efficient method. The parallel region is created only once, and each thread takes a large chunk of the outer loop's iterations to work on. This minimizes thread creation overhead.

```mermaid
graph TD
    A["#pragma omp parallel for"] --> B{i-Loop<br>(e.g., i=0 to 499)};

    subgraph "Single Parallel Region (Low Overhead)"
        B -- "i=0..124" --> T1["Thread 0<br>Executes inner j-loop and k-loop<br>for its assigned 'i' values"];
        B -- "i=125..249" --> T2["Thread 1<br>..."];
        B -- "i=250..374" --> T3["Thread 2<br>..."];
        B -- "i=375..499" --> T4["Thread 3<br>..."];
    end

    style A fill:#f9f,stroke:#333,stroke-width:2px
```

#### 2. Inner Loop Parallelization

This method is less efficient. A new parallel region is created for _each iteration_ of the outer loop. The repeated creation and destruction of threads leads to very high overhead.

```mermaid
graph TD
    A{Outer i-Loop} --> B{"For i = 0"};
    B --> C_i0["#pragma omp parallel for"];

    subgraph "Parallel Region for i=0 (High Overhead)"
        C_i0 -- "j=0..124" --> T1_i0["Thread 0"];
        C_i0 -- "j=125..249" --> T2_i0["Thread 1"];
        C_i0 -- "..." --> TN_i0["..."];
    end

    A --> D{"For i = 1"};
    D --> C_i1["#pragma omp parallel for"];
     subgraph "Parallel Region for i=1 (High Overhead)"
        C_i1 -- "j=0..124" --> T1_i1["Thread 0"];
        C_i1 -- "j=125..249" --> T2_i1["Thread 1"];
        C_i1 -- "..." --> TN_i1["..."];
    end

    A --> E{"..."};

    style C_i0 fill:#f8d7da,stroke:#721c24,stroke-width:2px
    style C_i1 fill:#f8d7da,stroke:#721c24,stroke-width:2px
```

---

## 📷 Screenshot Documentation Guide

### **Screenshot Organization Structure**

<<<<<<< HEAD
#### System Configuration

| Parameter              | Value                                      |
| ---------------------- | ------------------------------------------ |
| **Hostname**           | Ubuntor                                    |
| **System**             | Linux 6.11.0-29-generic                    |
| **Architecture**       | x86_64                                     |
| **CPU Model**          | AMD Ryzen 7 8845HS w/ Radeon 780M Graphics |
| **Logical Cores**      | 8                                          |
| **Max OpenMP Threads** | 8                                          |
| **OpenMP Version**     | 201511                                     |

#### Thread Creation Performance

| Configuration                  | Threads Requested | Threads Created | Execution Time (s) | Status   |
| ------------------------------ | ----------------- | --------------- | ------------------ | -------- |
| **10 Threads (hardcoded)**     | 10                | 10              | 0.007688           | ✓ PASSED |
| **User-specified (4 threads)** | 4                 | 4               | 0.006180           | ✓ PASSED |
| **Dynamic scaling test**       | 2-8               | 2-8             | Variable           | ✓ PASSED |

### Part B: Vector Addition Performance Comparison

#### Performance Summary by Vector Size and Scheduling

**100K Elements Performance (Best Times)**

| Threads | Static (s) | Dynamic (s)  | Guided (s) | Auto (s)     | Best Method |
| ------- | ---------- | ------------ | ---------- | ------------ | ----------- |
| 1       | 0.001143   | **0.000244** | 0.000785   | 0.000872     | Dynamic     |
| 2       | 0.001002   | 0.000777     | 0.000046   | **0.000044** | Auto        |
| 4       | 0.000442   | **0.000023** | 0.000029   | 0.000038     | Dynamic     |
| 8       | 0.009209   | 0.010135     | 0.007205   | **0.005147** | Auto        |

**1M Elements Performance (Best Times)**

| Threads | Static (s)   | Dynamic (s) | Guided (s) | Auto (s)     | Best Method |
| ------- | ------------ | ----------- | ---------- | ------------ | ----------- |
| 1       | **0.003756** | 0.003973    | 0.008083   | 0.008107     | Static      |
| 2       | 0.003373     | 0.003582    | 0.002552   | **0.001299** | Auto        |
| 4       | 0.001121     | 0.002191    | 0.000745   | **0.000585** | Auto        |
| 8       | 0.009510     | 0.013030    | 0.010484   | **0.008552** | Auto        |

**5M Elements Performance (Best Times)**

| Threads | Static (s)   | Dynamic (s)  | Guided (s)   | Auto (s) | Best Method |
| ------- | ------------ | ------------ | ------------ | -------- | ----------- |
| 1       | **0.032049** | 0.055740     | 0.287245     | 0.277620 | Static      |
| 2       | 0.010121     | 0.009767     | **0.007195** | 0.009718 | Guided      |
| 4       | 0.010691     | 0.009246     | **0.006599** | 0.009990 | Guided      |
| 8       | 0.015181     | **0.012254** | 0.012584     | 0.024319 | Dynamic     |

#### Key Findings for Vector Addition

- **Static scheduling** performs best for single-threaded execution and very large datasets
- **Dynamic scheduling** excels with moderate thread counts (2-4 threads)
- **Auto scheduling** shows strong performance across various configurations
- **Guided scheduling** provides the best balance for medium to large datasets
- Performance scaling varies significantly with vector size and thread count

### Part B: Additional Static vs Dynamic Scheduling Analysis

#### Static vs Dynamic (Chunk Size 1) Performance Comparison

**Performance Summary: Static vs Dynamic(chunk=1)**

| Vector Size | Threads | Static Time (ms) | Dynamic(1) Time (ms) | Performance Winner | Overhead % |
| ----------- | ------- | ---------------- | -------------------- | ------------------ | ---------- |
| **1K**      | 1       | 0.0013           | 0.0045               | Static             | +237.9%    |
|             | 2       | 0.0076           | 0.0305               | Static             | +299.2%    |
|             | 4       | 0.0009           | 0.0378               | Static             | +4114.8%   |
|             | 8       | 7.0123           | 6.9552               | Dynamic(1)         | +0.8%      |
| **10K**     | 1       | 0.0047           | 0.0383               | Static             | +717.7%    |
|             | 2       | 0.0175           | 0.3553               | Static             | +1930.3%   |
|             | 4       | 0.0029           | 0.2885               | Static             | +9682.0%   |
|             | 8       | 4.8675           | 0.1533               | Dynamic(1)         | +3075.2%   |
| **50K**     | 1       | 0.0858           | 0.2479               | Static             | +189.1%    |
|             | 2       | 0.0979           | 1.2108               | Static             | +1136.5%   |
|             | 4       | 0.0271           | 1.3628               | Static             | +4926.0%   |
|             | 8       | 9.6709           | 10.1578              | Static             | +5.0%      |
| **100K**    | 1       | 0.0915           | 0.6272               | Static             | +585.4%    |
|             | 2       | 0.2052           | 2.2149               | Static             | +979.2%    |
|             | 4       | 0.0659           | 2.5261               | Static             | +3734.7%   |
|             | 8       | 9.9846           | 7.9956               | Dynamic(1)         | +24.9%     |
| **500K**    | 1       | 1.5138           | 2.6568               | Static             | +75.5%     |
|             | 2       | 1.4972           | 8.2759               | Static             | +452.8%    |
|             | 4       | 0.4020           | 9.8781               | Static             | +2357.1%   |
|             | 8       | 0.2119           | 7.6347               | Static             | +3502.4%   |

#### Static vs Dynamic (Optimized Chunk Size) Performance Comparison

**Performance Summary: Static vs Dynamic(optimized chunks)**

| Vector Size | Threads | Static Time (ms) | Dynamic Time (ms) | Chunk Size | Winner  | Performance Diff % |
| ----------- | ------- | ---------------- | ----------------- | ---------- | ------- | ------------------ |
| **1K**      | 1       | 0.0031           | 0.0014            | 100        | Dynamic | +114.3%            |
|             | 2       | 0.5959           | 0.0015            | 50         | Dynamic | +39339.2%          |
|             | 4       | 0.4229           | 0.0017            | 25         | Dynamic | +24772.6%          |
|             | 8       | 4.3259           | 0.0339            | 12         | Dynamic | +12678.1%          |
| **10K**     | 1       | 0.0307           | 0.0084            | 1000       | Dynamic | +266.5%            |
|             | 2       | 0.0041           | 0.0028            | 500        | Dynamic | +43.1%             |
|             | 4       | 0.6293           | 0.0069            | 250        | Dynamic | +9067.4%           |
|             | 8       | 7.3363           | 6.5429            | 125        | Dynamic | +12.1%             |
| **50K**     | 1       | 0.0340           | 0.0300            | 1000       | Dynamic | +13.4%             |
|             | 2       | 0.0393           | 0.0271            | 1000       | Dynamic | +44.9%             |
|             | 4       | 0.0855           | 0.0075            | 1000       | Dynamic | +1039.6%           |
|             | 8       | 4.1696           | 0.0089            | 625        | Dynamic | +46636.1%          |
| **100K**    | 1       | 0.2447           | 0.6438            | 1000       | Static  | +163.1%            |
|             | 2       | 1.0089           | 0.0853            | 1000       | Dynamic | +1082.7%           |
|             | 4       | 0.2332           | 0.0495            | 1000       | Dynamic | +370.8%            |
|             | 8       | 8.6645           | 8.6485            | 1000       | Dynamic | +0.2%              |

#### Chunk Size Optimization Analysis

**Dynamic Scheduling Chunk Size Performance (10K Elements, 4 Threads)**

| Chunk Size | Avg Time (ms) | Std Dev (ms) | Throughput (elements/s) | Overhead vs Chunk=1 |
| ---------- | ------------- | ------------ | ----------------------- | ------------------- |
| 1          | 0.6022        | 1.0483       | 1.66×10⁷                | Baseline            |
| 10         | 0.1062        | 0.0998       | 9.41×10⁷                | -82.4%              |
| 100        | 0.0041        | 0.0005       | 2.45×10⁹                | -99.3%              |
| 500        | 0.0021        | 0.0001       | 4.78×10⁹                | -99.7%              |
| 1000       | 0.0020        | 0.0001       | 5.12×10⁹                | -99.7%              |
| 2000       | 0.0018        | 0.0001       | 5.66×10⁹                | -99.7%              |

**Dynamic Scheduling Chunk Size Performance (100K Elements, 4 Threads)**

| Chunk Size | Avg Time (ms) | Std Dev (ms) | Throughput (elements/s) | Overhead vs Chunk=1 |
| ---------- | ------------- | ------------ | ----------------------- | ------------------- |
| 1          | 2.4607        | 0.9372       | 4.06×10⁷                | Baseline            |
| 10         | 1.0476        | 1.5385       | 9.55×10⁷                | -57.4%              |
| 100        | 0.0368        | 0.0029       | 2.72×10⁹                | -98.5%              |
| 500        | 0.0159        | 0.0025       | 6.30×10⁹                | -99.4%              |
| 1000       | 0.0112        | 0.0008       | 8.95×10⁹                | -99.5%              |
| 2000       | 0.0108        | 0.0008       | 9.23×10⁹                | -99.6%              |

#### Analysis of Virtualization and Hardware Effects

**System Anomalies Observed:**

1. **Static Scheduling Inconsistency**: Highly variable performance (1000x+ differences) for static scheduling with small vectors
2. **Thread Count Dependencies**: 8-thread performance sometimes favors dynamic due to system interference
3. **Virtualization Overhead**: VirtualBox environment may cause thread scheduling irregularities
4. **Memory Contention**: Evidence of memory bandwidth limitations affecting static scheduling

**Potential Virtualization Impact:**

| Factor                | Impact on Static | Impact on Dynamic  | Explanation                                             |
| --------------------- | ---------------- | ------------------ | ------------------------------------------------------- |
| **CPU Scheduling**    | High variability | More predictable   | VM scheduler may interrupt static threads mid-execution |
| **Memory Access**     | Cache conflicts  | Better adaptation  | Dynamic adapts to VM memory allocation patterns         |
| **System Load**       | Poor adaptation  | Good adaptation    | Static assumes dedicated resources, dynamic adjusts     |
| **Thread Priorities** | Assumes equality | Handles variations | VM may deprioritize certain threads                     |

#### Scheduling Overhead Analysis

**Synchronization Costs (Based on Chunk Size 1 Data):**

| Vector Size | Sync Points | Cost per Sync (ns) | Total Overhead (ms) | Overhead % of Total |
| ----------- | ----------- | ------------------ | ------------------- | ------------------- |
| 1K          | 1,000       | 36.9               | 0.037               | 97.6%               |
| 10K         | 10,000      | 28.6               | 0.286               | 99.0%               |
| 50K         | 50,000      | 26.7               | 1.336               | 98.0%               |
| 100K        | 100,000     | 24.6               | 2.460               | 97.4%               |
| 500K        | 500,000     | 19.0               | 9.476               | 95.9%               |

**Key Findings:**

- **Synchronization cost**: ~20-40 nanoseconds per work assignment
- **Overhead dominance**: 95-99% of execution time in Dynamic(chunk=1)
- **Chunk size optimization**: Critical for dynamic scheduling performance
- **System adaptation**: Dynamic scheduling better handles virtualized environments

### Part C: Matrix Calculation Performance Analysis

#### 50x50 Matrix Performance

| Parallelization | Threads | Avg. Time (s) | GFLOPS | Speedup | Efficiency | Best Time (s) |
| --------------- | ------- | ------------- | ------ | ------- | ---------- | ------------- |
| **Outer Loop**  | 1       | 0.000064      | 3.91   | 1.00x   | 100.0%     | 0.000062      |
|                 | 4       | 0.000441      | 0.57   | 0.14x   | 3.6%       | 0.000017      |
|                 | 8       | 0.008546      | 0.03   | 0.01x   | 0.1%       | 0.003670      |
|                 | 16      | 0.002455      | 0.10   | 0.03x   | 0.2%       | 0.000808      |
| **Inner Loop**  | 1       | 0.000080      | 3.13   | 1.00x   | 100.0%     | 0.000078      |
|                 | 4       | 0.000073      | 3.44   | 1.10x   | 27.5%      | 0.000040      |
|                 | 8       | 0.087795      | 0.00   | 0.00x   | 0.0%       | 0.000045      |
|                 | 16      | 0.139354      | 0.00   | 0.00x   | 0.0%       | 0.100196      |

#### 500x500 Matrix Performance

| Parallelization | Threads | Avg. Time (s) | GFLOPS | Speedup | Efficiency | Best Time (s) |
| --------------- | ------- | ------------- | ------ | ------- | ---------- | ------------- |
| **Outer Loop**  | 1       | 1.164745      | 0.21   | 1.00x   | 100.0%     | 1.139651      |
|                 | 4       | 0.288332      | 0.87   | 4.04x   | 101.0%     | 0.272568      |
|                 | 8       | 0.163254      | 1.53   | 7.13x   | 89.2%      | 0.113722      |
|                 | 16      | 0.181919      | 1.37   | 6.40x   | 40.0%      | 0.156086      |
| **Inner Loop**  | 1       | 1.168256      | 0.21   | 1.00x   | 100.0%     | 1.151926      |
|                 | 4       | 0.346331      | 0.72   | 3.37x   | 84.3%      | 0.210345      |
|                 | 8       | 0.121934      | 2.05   | 9.58x   | 119.8%     | 0.102996      |
|                 | 16      | 1.578263      | 0.16   | 0.74x   | 4.6%       | 1.526112      |

#### 1000x1000 Matrix Performance

| Parallelization | Threads | Avg. Time (s) | GFLOPS | Speedup | Efficiency | Best Time (s) |
| --------------- | ------- | ------------- | ------ | ------- | ---------- | ------------- |
| **Outer Loop**  | 1       | 1.327500      | 1.51   | 1.00x   | 100.0%     | 0.865626      |
|                 | 4       | 0.419517      | 4.77   | 3.16x   | 79.1%      | 0.346475      |
|                 | 8       | 0.315880      | 6.33   | 4.20x   | 52.5%      | 0.264190      |
|                 | 16      | 0.287860      | 6.95   | 4.61x   | 28.8%      | 0.236810      |
| **Inner Loop**  | 1       | 1.348493      | 1.48   | 1.00x   | 100.0%     | 0.945710      |
|                 | 4       | 0.436323      | 4.58   | 3.09x   | 77.3%      | 0.236550      |
|                 | 8       | 0.459771      | 4.35   | 2.93x   | 36.7%      | 0.242730      |
|                 | 16      | 3.455038      | 0.58   | 0.39x   | 2.4%       | 3.270360      |

#### Matrix Calculation Winner Analysis

| Matrix Size | Thread Count | Winner     | Performance Advantage | Reason                   |
| ----------- | ------------ | ---------- | --------------------- | ------------------------ |
| 50x50       | 1            | Outer Loop | 19.9% faster          | Lower overhead           |
|             | 4            | Inner Loop | 83.5% faster          | Better load balancing    |
|             | 8+           | Outer Loop | >90% faster           | Thread creation overhead |
| 500x500     | 1-4          | Outer Loop | 0.3-16.7% faster      | Memory locality          |
|             | 8            | Inner Loop | 25.3% faster          | Optimal parallelization  |
|             | 16           | Outer Loop | 88.5% faster          | Excessive overhead       |
| 1000x1000   | All          | Outer Loop | 1.6-91.7% faster      | Consistent superiority   |

#### Key Findings for Matrix Calculation

- **Outer loop parallelization** is generally superior for larger matrices
- **Inner loop parallelization** can be competitive for medium matrices with moderate thread counts
- **Thread overhead** becomes critical with smaller matrices
- **Scalability** improves significantly with larger problem sizes
- **Optimal thread count** typically matches the number of physical CPU cores
- **Diminishing returns** observed beyond 8 threads due to system limitations

### Performance Optimization Recommendations

#### For Vector Addition:

1. **Small vectors (<1M elements)**: Use dynamic or auto scheduling
2. **Large vectors (>5M elements)**: Static or guided scheduling preferred
3. **Thread count**: Optimal at 4-8 threads for this system
4. **Chunk size tuning**: Critical for dynamic scheduling performance

#### For Matrix Multiplication:

1. **Small matrices (<100x100)**: Single-threaded or 4 threads maximum
2. **Medium matrices (500x500)**: 8 threads with outer loop parallelization
3. **Large matrices (>1000x1000)**: Outer loop parallelization strongly recommended
4. **Thread scaling**: Diminishing returns beyond physical core count

#### General Guidelines:

- **Match thread count to physical cores** (8 cores optimal for this system)
- **Consider memory bandwidth limitations** for large datasets
- **Profile actual performance** rather than assuming theoretical scaling
- **Account for system load** and other running processes
=======
Create the following directory structure for your screenshots:

```
SCREENSHOTS/
├── PART_A_HELLO_WORLD/
│   ├── 01_system_verification.png
│   ├── 02_thread_creation_demo.png
│   ├── 03_private_shared_variables.png
│   └── 04_compilation_success.png
├── PART_B_VECTOR_ADDITION/
│   ├── 05_scheduling_comparison.png
│   ├── 06_vector_size_scaling.png
│   ├── 07_chunk1_overhead_demo.png
│   ├── 08_chunk_optimization.png
│   └── 09_static_vs_dynamic_results.png
├── PART_C_MATRIX_CALCULATION/
│   ├── 10_outer_vs_inner_comparison.png
│   ├── 11_thread_scaling_analysis.png
│   ├── 12_large_matrix_performance.png
│   └── 13_gflops_efficiency.png
├── CODE_SNIPPETS/
│   ├── 14_part_a_key_code.png
│   ├── 15_part_b_scheduling_code.png
│   ├── 16_part_c_parallelization_code.png
│   └── 17_compilation_commands.png
└── SUPPLEMENTARY/
    ├── 18_cpu_utilization.png
    ├── 19_performance_variation.png
    └── 20_system_specs.png
```
>>>>>>> 4ebf073c66d14b6dd986b66e76239a02bdf082f1

---

## 📸 **Part A: Enhanced OpenMP Hello World - Screenshot Requirements**

### **Screenshot 01: System Verification Output**
**What to capture**: Terminal output showing:
```
System: Linux 6.11.0-29-generic x86_64
CPU: AMD Ryzen 7 8845HS w/ Radeon 780M Graphics
Logical cores: 8
Max OpenMP threads: 8
OpenMP version: 201511
```
**Why important**: Establishes testing environment and hardware baseline

### **Screenshot 02: Thread Creation Demonstration**
**What to capture**: Program output showing:
- 10-thread hardcoded execution with thread IDs
- User-specified thread count (e.g., 4 threads)
- Execution times for both configurations
**Why important**: Proves OpenMP thread management works correctly

### **Screenshot 03: Private vs Shared Variables Demo**
**What to capture**: Output from `demonstrate_advanced_features()` showing:
- Private variable values per thread (should be different)
- Shared reduction results (should be consistent)
- Critical section demonstration
**Why important**: Demonstrates understanding of OpenMP variable scoping

### **Screenshot 04: Compilation Success**
**What to capture**: Terminal showing successful compilation:
```bash
gcc -fopenmp -O2 -o part_a_enhanced part_a_enhanced.c -lm
echo $?  # Should show 0 for success
```

---

## 📸 **Part B: Enhanced Vector Addition - Screenshot Requirements**

### **Screenshot 05: Scheduling Strategy Comparison**
**What to capture**: Side-by-side output showing all 4 scheduling types:
- Static scheduling results
- Dynamic scheduling results  
- Guided scheduling results
- Auto scheduling results
**Include**: Vector size, thread count, execution times for each strategy

### **Screenshot 06: Vector Size Scaling Results**
**What to capture**: Performance across multiple vector sizes:
- 100K elements performance
- 1M elements performance  
- 5M elements performance
**Show**: How execution time scales with problem size

### **Screenshot 07: Chunk Size 1 Overhead Demonstration**
**What to capture**: The extreme overhead comparison:
```
Static: 0.0029ms vs Dynamic(chunk=1): 0.2885ms
Overhead: +9682% slower!
```
**Why important**: Visual evidence of synchronization overhead

### **Screenshot 08: Chunk Size Optimization**
**What to capture**: Dynamic scheduling with different chunk sizes:
- Chunk=1: 0.6022ms
- Chunk=100: 0.0041ms  
- Chunk=1000: 0.0020ms
- Chunk=2000: 0.0018ms
**Show**: Performance improvement from chunk size tuning

### **Screenshot 09: Static vs Dynamic Comprehensive Results**
**What to capture**: Complete comparison table showing:
- Multiple vector sizes (1K, 10K, 50K, 100K, 500K)
- Thread scaling (1, 2, 4, 8 threads)
- Performance winner for each configuration

---

## 📸 **Part C: Enhanced Matrix Calculation - Screenshot Requirements**

### **Screenshot 10: Outer vs Inner Loop Comparison**
**What to capture**: Side-by-side performance results:
- Outer loop parallelization times
- Inner loop parallelization times
- GFLOPS calculations for both
- Speedup and efficiency metrics

### **Screenshot 11: Thread Scaling Analysis**
**What to capture**: Performance across thread counts:
- 1, 4, 8, 16 threads for 500x500 matrix
- Execution times, speedup ratios
- Efficiency percentages
**Show**: Diminishing returns beyond optimal thread count

### **Screenshot 12: Large Matrix Performance (1000x1000)**
**What to capture**: Best-case performance results:
- Outer loop: 6.95 GFLOPS, 4.61x speedup
- Inner loop: Poor scaling with 16 threads
- Clear winner demonstration

### **Screenshot 13: GFLOPS and Efficiency Metrics**
**What to capture**: Complete performance table showing:
- Matrix sizes (50x50, 500x500, 1000x1000)
- GFLOPS calculations
- Efficiency percentages (0.1% to 119.8%)

---

## 💻 **Code Snippets to Screenshot**

### **Screenshot 14: Part A Key Code Sections**
**File**: `part_a_enhanced.c`
**Sections to capture**:

1. **OpenMP Parallel Region**:
```c
#pragma omp parallel
{
    int thread_id = omp_get_thread_num();
    int total_threads = omp_get_num_threads();
    printf("Hello from thread %d of %d\n", thread_id, total_threads);
}
```

2. **Advanced Features Function**:
```c
void demonstrate_advanced_features() {
    #pragma omp parallel private(private_var) shared(shared_sum)
    {
        // Private and shared variable demonstration
        #pragma omp for reduction(+:shared_sum)
        for (int i = 0; i < 1000; i++) {
            shared_sum += i;
        }
    }
}
```

### **Screenshot 15: Part B Scheduling Code**
**File**: `part_b_enhanced.c`
**Sections to capture**:

1. **Static Scheduling**:
```c
#pragma omp parallel for schedule(static)
for (int i = 0; i < vector_size; i++) {
    result[i] = vector_a[i] + vector_b[i];
}
```

2. **Dynamic Scheduling with Chunk Size**:
```c
#pragma omp parallel for schedule(dynamic, chunk_size)
for (int i = 0; i < vector_size; i++) {
    result[i] = vector_a[i] + vector_b[i];
}
```

3. **Performance Measurement Code**:
```c
start_time = omp_get_wtime();
// Parallel computation here
end_time = omp_get_wtime();
execution_time = end_time - start_time;
```

### **Screenshot 16: Part C Parallelization Code**
**File**: `part_c_enhanced.c`
**Sections to capture**:

1. **Outer Loop Parallelization**:
```c
#pragma omp parallel for
for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
        for (int k = 0; k < size; k++) {
            result[i][j] += matrix_a[i][k] * matrix_b[k][j];
        }
    }
}
```

2. **Inner Loop Parallelization**:
```c
for (int i = 0; i < size; i++) {
    #pragma omp parallel for
    for (int j = 0; j < size; j++) {
        for (int k = 0; k < size; k++) {
            result[i][j] += matrix_a[i][k] * matrix_b[k][j];
        }
    }
}
```

3. **GFLOPS Calculation**:
```c
double gflops = (2.0 * size * size * size) / (execution_time * 1e9);
double speedup = serial_time / parallel_time;
double efficiency = (speedup / num_threads) * 100.0;
```

### **Screenshot 17: Compilation Commands**
**What to capture**: Terminal showing compilation process:
```bash
# Part A compilation
gcc -fopenmp -O2 -o part_a_enhanced part_a_enhanced.c -lm

# Part B compilation  
gcc -fopenmp -O2 -o part_b_enhanced part_b_enhanced.c -lm

# Part C compilation
gcc -fopenmp -O2 -o part_c_enhanced part_c_enhanced.c -lm

# Static vs Dynamic programs
gcc -fopenmp -O2 -o part_b_static_vs_dynamic_chunk1 part_b_static_vs_dynamic_chunk1.c -lm
gcc -fopenmp -O2 -o part_b_vector_static_vs_dynamic part_b_vector_static_vs_dynamic.c -lm
```

---

## 📊 **Supplementary Screenshots**

### **Screenshot 18: CPU Utilization During Parallel Execution**
**What to capture**: `htop` or `top` output showing:
- Multiple CPU cores active during parallel execution
- CPU usage percentages across cores
- Memory utilization
**Command**: `htop` while running parallel programs

### **Screenshot 19: Performance Variation Evidence**
**What to capture**: Multiple runs of the same test showing:
- Variation in static scheduling performance
- Consistent dynamic scheduling performance
- Statistical evidence supporting virtualization hypothesis

### **Screenshot 20: Complete System Specifications**
**What to capture**: System information commands:
```bash
lscpu | head -20
cat /proc/version
free -h
uname -a
```

---

## 🎯 **Screenshot Best Practices**

### **Technical Guidelines**:
1. **Use clear terminal fonts** (Ubuntu Mono, Consolas, or similar)
2. **Set appropriate zoom level** for readability (at least 12pt font)
3. **Use consistent terminal size** across all screenshots
4. **Include command prompts** to show what was executed
5. **Capture full output** including timing measurements
6. **Use terminal selection** to highlight key numbers if possible

### **Content Guidelines**:
1. **Show timestamps** when experiments were run
2. **Include system context** (working directory, user, etc.)
3. **Capture compilation success** (return codes, no errors)
4. **Document exact commands used** for reproducibility
5. **Include performance metrics** clearly visible
6. **Show statistical variations** where relevant

### **Organization Guidelines**:
1. **Name files descriptively** with numbers for ordering
2. **Group by portfolio part** for easy navigation
3. **Include both output and code** screenshots
4. **Create a master index** of all screenshots
5. **Backup originals** in high resolution
6. **Export in common formats** (PNG preferred for quality)

This comprehensive screenshot guide will provide compelling visual evidence of your parallel programming mastery and support all performance claims in your analysis!
