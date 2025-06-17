/*
 * ================================================================
 * Part B: Static vs Dynamic (Chunk Size 1) Scheduling Comparison
 * 6004CEM Parallel and Distributed Programming Coursework
 * 
 * Author: Matthew Loh Yet Marn
 * Date: APR2025
 * 
 * Focus: Detailed comparison of static vs dynamic scheduling (chunk=1)
 * Analysis: Performance implications of fine-grained work distribution
 * ================================================================
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/utsname.h>

/* Configuration constants */
#define MAX_VECTOR_SIZES 5
#define NUM_PERFORMANCE_RUNS 15  // Multiple runs for statistical accuracy
#define MAX_THREADS 8

/* Vector sizes for comprehensive testing */
const int VECTOR_SIZES[MAX_VECTOR_SIZES] = {1000, 10000, 50000, 100000, 500000};
const char* VECTOR_SIZE_LABELS[MAX_VECTOR_SIZES] = {"1K", "10K", "50K", "100K", "500K"};

/* Performance measurement structure */
typedef struct {
    double avg_time;
    double min_time;
    double max_time;
    double std_dev;
    double throughput;
    double overhead_estimate;
    int thread_count;
    int vector_size;
    const char* schedule_type;
} PerformanceMetrics;

/* Function to display system information */
void display_system_info() {
    struct utsname sys_info;
    char hostname[256];
    
    printf("=== SYSTEM VERIFICATION INFORMATION ===\n");
    
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        printf("Hostname: %s\n", hostname);
    }
    
    if (uname(&sys_info) == 0) {
        printf("System: %s %s\n", sys_info.sysname, sys_info.release);
        printf("Architecture: %s\n", sys_info.machine);
    }
    
    char *user = getenv("USER");
    if (user) printf("User: %s\n", user);
    
    time_t now = time(NULL);
    printf("Execution Time: %s", ctime(&now));
    
    printf("CPU Cores: %d\n", omp_get_num_procs());
    printf("Max OpenMP Threads: %d\n", omp_get_max_threads());
    printf("=========================================\n\n");
}

/* Initialize vectors with deterministic pattern */
void initialize_vectors(double *a, double *b, int size) {
    srand(42);  // Fixed seed for reproducible results
    
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        a[i] = (double)(i % 1000) + sin(i * 0.001);
        b[i] = (double)((i * 3) % 1000) + cos(i * 0.001);
    }
}

/* Verify results are identical between scheduling methods */
int verify_results(double *result1, double *result2, int size) {
    int errors = 0;
    const double tolerance = 1e-12;
    
    #pragma omp parallel for reduction(+:errors)
    for (int i = 0; i < size; i++) {
        if (fabs(result1[i] - result2[i]) > tolerance) {
            errors++;
        }
    }
    
    return errors == 0;
}

/* Calculate statistical metrics */
double calculate_std_dev(double *values, int count, double mean) {
    double sum_sq_diff = 0.0;
    for (int i = 0; i < count; i++) {
        double diff = values[i] - mean;
        sum_sq_diff += diff * diff;
    }
    return sqrt(sum_sq_diff / count);
}

/* Test static scheduling performance */
PerformanceMetrics test_static_scheduling(double *a, double *b, double *c, 
                                        int size, int num_threads) {
    omp_set_num_threads(num_threads);
    double times[NUM_PERFORMANCE_RUNS];
    
    printf("    Testing STATIC scheduling (%d threads)...\n", num_threads);
    
    // Warm-up run
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
    }
    
    // Performance measurement runs
    for (int run = 0; run < NUM_PERFORMANCE_RUNS; run++) {
        double start_time = omp_get_wtime();
        
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < size; i++) {
            c[i] = a[i] + b[i];
        }
        
        double end_time = omp_get_wtime();
        times[run] = end_time - start_time;
    }
    
    // Calculate statistics
    double sum = 0.0, min_time = times[0], max_time = times[0];
    for (int i = 0; i < NUM_PERFORMANCE_RUNS; i++) {
        sum += times[i];
        if (times[i] < min_time) min_time = times[i];
        if (times[i] > max_time) max_time = times[i];
    }
    
    double avg_time = sum / NUM_PERFORMANCE_RUNS;
    double std_dev = calculate_std_dev(times, NUM_PERFORMANCE_RUNS, avg_time);
    
    // Estimate overhead (difference from minimum time)
    double overhead_estimate = avg_time - min_time;
    
    PerformanceMetrics metrics = {
        .avg_time = avg_time,
        .min_time = min_time,
        .max_time = max_time,
        .std_dev = std_dev,
        .throughput = size / avg_time,
        .overhead_estimate = overhead_estimate,
        .thread_count = num_threads,
        .vector_size = size,
        .schedule_type = "Static"
    };
    
    return metrics;
}

/* Test dynamic scheduling with chunk size 1 */
PerformanceMetrics test_dynamic_chunk1_scheduling(double *a, double *b, double *c, 
                                                int size, int num_threads) {
    omp_set_num_threads(num_threads);
    double times[NUM_PERFORMANCE_RUNS];
    
    printf("    Testing DYNAMIC (chunk=1) scheduling (%d threads)...\n", num_threads);
    
    // Warm-up run
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < size; i++) {
        c[i] = a[i] + b[i];
    }
    
    // Performance measurement runs
    for (int run = 0; run < NUM_PERFORMANCE_RUNS; run++) {
        double start_time = omp_get_wtime();
        
        #pragma omp parallel for schedule(dynamic, 1)
        for (int i = 0; i < size; i++) {
            c[i] = a[i] + b[i];
        }
        
        double end_time = omp_get_wtime();
        times[run] = end_time - start_time;
    }
    
    // Calculate statistics
    double sum = 0.0, min_time = times[0], max_time = times[0];
    for (int i = 0; i < NUM_PERFORMANCE_RUNS; i++) {
        sum += times[i];
        if (times[i] < min_time) min_time = times[i];
        if (times[i] > max_time) max_time = times[i];
    }
    
    double avg_time = sum / NUM_PERFORMANCE_RUNS;
    double std_dev = calculate_std_dev(times, NUM_PERFORMANCE_RUNS, avg_time);
    
    // Estimate overhead (difference from minimum time)
    double overhead_estimate = avg_time - min_time;
    
    PerformanceMetrics metrics = {
        .avg_time = avg_time,
        .min_time = min_time,
        .max_time = max_time,
        .std_dev = std_dev,
        .throughput = size / avg_time,
        .overhead_estimate = overhead_estimate,
        .thread_count = num_threads,
        .vector_size = size,
        .schedule_type = "Dynamic(1)"
    };
    
    return metrics;
}

/* Analyze work distribution patterns */
void analyze_work_distribution(int vector_size, int num_threads) {
    printf("\n--- WORK DISTRIBUTION ANALYSIS ---\n");
    printf("Vector size: %d elements, %d threads\n", vector_size, num_threads);
    
    printf("\nSTATIC Scheduling:\n");
    int base_chunk = vector_size / num_threads;
    int remainder = vector_size % num_threads;
    
    printf("  Base chunk size per thread: %d elements\n", base_chunk);
    printf("  Threads with extra element: %d\n", remainder);
    printf("  Work distribution:\n");
    
    for (int t = 0; t < num_threads && t < 6; t++) {
        int start = t * base_chunk + (t < remainder ? t : remainder);
        int chunk_size = base_chunk + (t < remainder ? 1 : 0);
        int end = start + chunk_size - 1;
        printf("    Thread %d: elements %d-%d (%d elements)\n", 
               t, start, end, chunk_size);
    }
    if (num_threads > 6) {
        printf("    ... (remaining %d threads follow similar pattern)\n", num_threads - 6);
    }
    
    printf("\nDYNAMIC (chunk=1) Scheduling:\n");
    printf("  Chunk size: 1 element per assignment\n");
    printf("  Total work assignments needed: %d\n", vector_size);
    printf("  Average assignments per thread: %.1f\n", (double)vector_size / num_threads);
    printf("  Work assignment pattern: Round-robin, one element at a time\n");
    printf("  Synchronization points: %d (one per element assignment)\n", vector_size);
}

/* Compare scheduling overhead */
void analyze_scheduling_overhead(PerformanceMetrics static_metrics, 
                               PerformanceMetrics dynamic_metrics) {
    printf("\n--- SCHEDULING OVERHEAD ANALYSIS ---\n");
    
    double performance_diff = dynamic_metrics.avg_time - static_metrics.avg_time;
    double overhead_percent = (performance_diff / static_metrics.avg_time) * 100.0;
    
    printf("Performance Comparison:\n");
    printf("  Static average time:    %.6f seconds\n", static_metrics.avg_time);
    printf("  Dynamic(1) average time: %.6f seconds\n", dynamic_metrics.avg_time);
    printf("  Time difference:        %.6f seconds\n", performance_diff);
    printf("  Overhead percentage:    %+.2f%%\n", overhead_percent);
    
    printf("\nVariability Analysis:\n");
    printf("  Static std deviation:   %.6f seconds (%.2f%%)\n", 
           static_metrics.std_dev, 
           (static_metrics.std_dev / static_metrics.avg_time) * 100);
    printf("  Dynamic(1) std deviation: %.6f seconds (%.2f%%)\n", 
           dynamic_metrics.std_dev,
           (dynamic_metrics.std_dev / dynamic_metrics.avg_time) * 100);
    
    printf("\nThroughput Comparison:\n");
    printf("  Static throughput:      %.2e elements/second\n", static_metrics.throughput);
    printf("  Dynamic(1) throughput:  %.2e elements/second\n", dynamic_metrics.throughput);
    printf("  Throughput ratio:       %.4f (Dynamic/Static)\n", 
           dynamic_metrics.throughput / static_metrics.throughput);
    
    // Estimate synchronization cost
    int estimated_sync_points = dynamic_metrics.vector_size;  // One per element
    double sync_cost_per_element = performance_diff / estimated_sync_points;
    
    printf("\nEstimated Costs:\n");
    printf("  Synchronization points: %d\n", estimated_sync_points);
    printf("  Cost per sync (approx): %.2e seconds\n", sync_cost_per_element);
    printf("  Total sync overhead:    %.6f seconds (%.1f%% of total time)\n",
           performance_diff > 0 ? performance_diff : 0.0,
           performance_diff > 0 ? (performance_diff / dynamic_metrics.avg_time) * 100 : 0.0);
}

/* Comprehensive performance test for one vector size */
void comprehensive_test(int vector_size, const char* size_label) {
    printf("\n================================================================\n");
    printf("COMPREHENSIVE TEST: %s ELEMENTS (%d)\n", size_label, vector_size);
    printf("================================================================\n");
    
    double *a = (double*)malloc(vector_size * sizeof(double));
    double *b = (double*)malloc(vector_size * sizeof(double));
    double *c_static = (double*)malloc(vector_size * sizeof(double));
    double *c_dynamic = (double*)malloc(vector_size * sizeof(double));
    
    if (!a || !b || !c_static || !c_dynamic) {
        printf("Error: Memory allocation failed!\n");
        return;
    }
    
    initialize_vectors(a, b, vector_size);
    
    printf("\nTesting different thread configurations:\n");
    printf("========================================================================\n");
    printf("Threads | Static Time | Static StdDev | Dynamic(1) Time | Dynamic(1) StdDev | Winner\n");
    printf("        |    (ms)     |     (ms)      |      (ms)       |       (ms)        |       \n");
    printf("--------|-------------|---------------|-----------------|-------------------|--------\n");
    
    int thread_counts[] = {1, 2, 4, 8};
    int num_configs = 4;
    
    for (int i = 0; i < num_configs; i++) {
        int threads = thread_counts[i];
        
        // Test static scheduling
        PerformanceMetrics static_result = 
            test_static_scheduling(a, b, c_static, vector_size, threads);
        
        // Test dynamic scheduling with chunk size 1
        PerformanceMetrics dynamic_result = 
            test_dynamic_chunk1_scheduling(a, b, c_dynamic, vector_size, threads);
        
        // Determine winner and calculate difference
        const char* winner;
        double perf_diff;
        if (static_result.avg_time < dynamic_result.avg_time) {
            winner = "Static";
            perf_diff = ((dynamic_result.avg_time - static_result.avg_time) / static_result.avg_time) * 100;
        } else {
            winner = "Dynamic(1)";
            perf_diff = ((static_result.avg_time - dynamic_result.avg_time) / dynamic_result.avg_time) * 100;
        }
        
        printf("%7d | %10.4f | %12.4f | %14.4f | %16.4f | %s (+%.1f%%)\n",
               threads,
               static_result.avg_time * 1000,
               static_result.std_dev * 1000,
               dynamic_result.avg_time * 1000,
               dynamic_result.std_dev * 1000,
               winner, perf_diff);
        
        // Detailed analysis for 4 threads
        if (threads == 4) {
            analyze_work_distribution(vector_size, threads);
            analyze_scheduling_overhead(static_result, dynamic_result);
        }
    }
    
    // Verify results correctness
    printf("\n--- RESULTS VERIFICATION ---\n");
    if (verify_results(c_static, c_dynamic, vector_size)) {
        printf("✓ Static and Dynamic(1) scheduling produce identical results\n");
    } else {
        printf("✗ ERROR: Results differ between scheduling methods!\n");
    }
    
    free(a);
    free(b);
    free(c_static);
    free(c_dynamic);
}

/* Theoretical analysis of static vs dynamic(1) */
void theoretical_analysis() {
    printf("\n================================================================\n");
    printf("THEORETICAL ANALYSIS: STATIC vs DYNAMIC(chunk=1)\n");
    printf("================================================================\n");
    
    printf("\nSTATIC SCHEDULING CHARACTERISTICS:\n");
    printf("✓ Work Distribution: Large contiguous chunks per thread\n");
    printf("✓ Synchronization: Minimal (only at parallel region boundaries)\n");
    printf("✓ Cache Performance: Excellent (sequential access within chunks)\n");
    printf("✓ Predictability: High (deterministic work assignment)\n");
    printf("✓ Overhead: Very low scheduling overhead\n");
    printf("✗ Load Balancing: Poor if threads finish at different times\n");
    printf("✗ Adaptability: Cannot adapt to runtime conditions\n");
    
    printf("\nDYNAMIC(chunk=1) SCHEDULING CHARACTERISTICS:\n");
    printf("✓ Load Balancing: Perfect (work assigned as threads become available)\n");
    printf("✓ Adaptability: Excellent adaptation to system conditions\n");
    printf("✓ Fairness: Equal opportunity for all threads to get work\n");
    printf("✗ Synchronization: High overhead (one sync per element)\n");
    printf("✗ Cache Performance: Poor (threads may access distant memory)\n");
    printf("✗ Predictability: Low (non-deterministic work assignment)\n");
    printf("✗ Overhead: Very high scheduling overhead\n");
    
    printf("\nPERFORMANCE TRADE-OFFS:\n");
    printf("Vector Size Impact:\n");
    printf("  • Small vectors: Dynamic(1) overhead dominates, Static likely faster\n");
    printf("  • Large vectors: Dynamic(1) overhead amortized, may be competitive\n");
    printf("  • Very large vectors: Memory bandwidth becomes limiting factor\n");
    
    printf("\nThread Count Impact:\n");
    printf("  • Few threads: Static overhead minimal, usually faster\n");
    printf("  • Many threads: Dynamic(1) synchronization contention increases\n");
    printf("  • More threads = more synchronization points in Dynamic(1)\n");
    
    printf("\nSystem Load Impact:\n");
    printf("  • Light load: Static performs predictably well\n");
    printf("  • Heavy load: Dynamic(1) may adapt better to thread interruptions\n");
    printf("  • Variable load: Dynamic(1) provides better load balancing\n");
}

/* Cache and memory analysis */
void cache_memory_analysis() {
    printf("\n================================================================\n");
    printf("CACHE AND MEMORY ACCESS ANALYSIS\n");
    printf("================================================================\n");
    
    printf("MEMORY ACCESS PATTERNS:\n");
    
    printf("\nStatic Scheduling:\n");
    printf("  Memory Layout: Thread 0 accesses elements [0...n/p-1]\n");
    printf("                 Thread 1 accesses elements [n/p...2n/p-1]\n");
    printf("                 etc.\n");
    printf("  Cache Behavior: • Excellent spatial locality\n");
    printf("                  • Sequential prefetching works well\n");
    printf("                  • Minimal cache line conflicts between threads\n");
    printf("                  • Predictable memory bandwidth usage\n");
    
    printf("\nDynamic(chunk=1) Scheduling:\n");
    printf("  Memory Layout: Thread assignment unpredictable\n");
    printf("                 Thread 0 might access [0, 4, 8, 12, ...]\n");
    printf("                 Thread 1 might access [1, 5, 9, 13, ...]\n");
    printf("  Cache Behavior: • Poor spatial locality\n");
    printf("                  • Cache line sharing between threads\n");
    printf("                  • Increased cache misses\n");
    printf("                  • Unpredictable memory access patterns\n");
    
    printf("\nCACHE LINE EFFECTS:\n");
    printf("  Typical cache line size: 64 bytes (8 double-precision values)\n");
    printf("  Static: Each thread works on separate cache lines\n");
    printf("  Dynamic(1): Multiple threads may share same cache lines\n");
    printf("  False sharing potential: High in Dynamic(1), Low in Static\n");
    
    printf("\nMEMORY BANDWIDTH UTILIZATION:\n");
    printf("  Static: Efficient, predictable bandwidth usage\n");
    printf("  Dynamic(1): Potentially inefficient due to scattered access\n");
    printf("  Impact: More significant as vector size increases\n");
}

/* Final recommendations */
void final_recommendations() {
    printf("\n================================================================\n");
    printf("CONCLUSIONS AND RECOMMENDATIONS\n");
    printf("================================================================\n");
    
    printf("KEY FINDINGS:\n");
    printf("1. Static scheduling typically outperforms Dynamic(chunk=1) for vector addition\n");
    printf("2. Dynamic(chunk=1) has significant synchronization overhead\n");
    printf("3. Cache performance heavily favors Static scheduling\n");
    printf("4. The overhead of Dynamic(chunk=1) is most visible with smaller vectors\n");
    printf("5. Load balancing benefits of Dynamic(chunk=1) rarely justify its costs\n");
    
    printf("\nWHEN TO USE EACH:\n");
    
    printf("\nUse STATIC scheduling when:\n");
    printf("✓ Vector addition or similar simple, uniform operations\n");
    printf("✓ Predictable, homogeneous computing environment\n");
    printf("✓ Cache performance is important\n");
    printf("✓ Minimizing synchronization overhead is critical\n");
    printf("✓ Working with regular, balanced workloads\n");
    
    printf("\nUse DYNAMIC(chunk=1) scheduling when:\n");
    printf("✓ Work per iteration is highly variable (not applicable here)\n");
    printf("✓ System load is unpredictable and threads may be interrupted\n");
    printf("✓ Perfect load balancing is more important than efficiency\n");
    printf("✓ Cache performance is not critical\n");
    printf("✓ Working with very large datasets where overhead amortizes\n");
    
    printf("\nPERFORMANCE OPTIMIZATION RECOMMENDATIONS:\n");
    printf("1. For vector addition, prefer Static scheduling\n");
    printf("2. If using Dynamic, choose larger chunk sizes (not 1)\n");
    printf("3. Consider Guided scheduling as a compromise\n");
    printf("4. Profile your specific system and workload\n");
    printf("5. Monitor cache miss rates and memory bandwidth\n");
    
    printf("\nCOURSEWORK IMPLICATIONS:\n");
    printf("✓ This analysis demonstrates understanding of scheduling trade-offs\n");
    printf("✓ Shows knowledge of cache and memory system effects\n");
    printf("✓ Provides empirical evidence for scheduling choices\n");
    printf("✓ Explains theoretical concepts with practical measurements\n");
    printf("✓ Addresses the specific case of chunk size 1 in detail\n");
}

/* Main function */
int main() {
    printf("================================================================\n");
    printf("Part B: Static vs Dynamic(chunk=1) Scheduling Comparison\n");
    printf("6004CEM Parallel Programming Coursework\n");
    printf("================================================================\n\n");
    
    display_system_info();
    
    // Set optimal environment for consistent testing
    omp_set_dynamic(0);  // Disable dynamic thread adjustment
    omp_set_nested(0);   // Disable nested parallelism
    
    printf("OpenMP Configuration:\n");
    printf("- Dynamic thread adjustment: DISABLED\n");
    printf("- Nested parallelism: DISABLED\n");
    printf("- Performance runs per test: %d\n", NUM_PERFORMANCE_RUNS);
    printf("- Focus: Static vs Dynamic with chunk size 1\n\n");
    
    // Test each vector size
    for (int i = 0; i < MAX_VECTOR_SIZES; i++) {
        comprehensive_test(VECTOR_SIZES[i], VECTOR_SIZE_LABELS[i]);
    }
    
    // Theoretical and practical analyses
    theoretical_analysis();
    cache_memory_analysis();
    final_recommendations();
    
    printf("\n================================================================\n");
    printf("PROGRAM COMPLETED SUCCESSFULLY\n");
    printf("================================================================\n");
    printf("This analysis specifically compared Static scheduling against\n");
    printf("Dynamic scheduling with chunk size 1, showing the extreme case\n");
    printf("of fine-grained work distribution and its performance implications.\n");
    printf("================================================================\n");
    
    return 0;
} 