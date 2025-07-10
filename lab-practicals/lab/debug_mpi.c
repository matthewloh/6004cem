#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int provided, required = MPI_THREAD_SINGLE;
    
    printf("Before MPI_Init: Process starting\n");
    fflush(stdout);
    
    // Initialize MPI
    int init_result = MPI_Init(&argc, &argv);
    if (init_result != MPI_SUCCESS) {
        printf("ERROR: MPI_Init failed with code %d\n", init_result);
        return 1;
    }
    
    printf("After MPI_Init: MPI initialized successfully\n");
    fflush(stdout);
    
    // Get rank and size
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    printf("Process %d of %d: MPI is working!\n", rank, size);
    fflush(stdout);
    
    // Barrier to synchronize output
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("\nSummary:\n");
        printf("  Total processes: %d\n", size);
        printf("  MPI_COMM_WORLD is working correctly\n");
        
        if (size >= 2) {
            printf("  ✓ Ready to run MPI programs with 2+ processes\n");
        } else {
            printf("  ✗ Need at least 2 processes for communication examples\n");
        }
    }
    
    MPI_Finalize();
    return 0;
} 