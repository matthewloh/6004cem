#include <stdio.h>
#include <mpi.h>
#include <stddef.h>

typedef struct {
    float x;
    int y;
    char name[20];
} MyStruct;

int main(int argc, char *argv[]) {
    int rank, size;
    MyStruct data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        printf("This program requires at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    // Step 1: Describe the structure using MPI derived type
    MPI_Datatype MPI_MyStruct;
    int block_lengths[3] = {1, 1, 20};  // One float, one int, 20 chars
    MPI_Aint displacements[3];
    MPI_Datatype types[3] = {MPI_FLOAT, MPI_INT, MPI_CHAR};

    // Compute displacements using offsetof
    displacements[0] = offsetof(MyStruct, x);
    displacements[1] = offsetof(MyStruct, y);
    displacements[2] = offsetof(MyStruct, name);

    // Create and commit the derived datatype
    MPI_Type_create_struct(3, block_lengths, displacements, types, &MPI_MyStruct);
    MPI_Type_commit(&MPI_MyStruct);

    if (rank == 0) {
        // Initialize data in process 0
        data.x = 3.14159;
        data.y = 42;
        sprintf(data.name, "Hello MPI");
        
        printf("Process 0 sending data:\n");
        printf("  x = %.5f\n", data.x);
        printf("  y = %d\n", data.y);
        printf("  name = %s\n", data.name);
        
        // Send the structured data to process 1
        MPI_Send(&data, 1, MPI_MyStruct, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 sent the structured data to process 1\n\n");
        
    } else if (rank == 1) {
        // Receive the structured data from process 0
        MPI_Recv(&data, 1, MPI_MyStruct, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Process 1 received data:\n");
        printf("  x = %.5f\n", data.x);
        printf("  y = %d\n", data.y);
        printf("  name = %s\n", data.name);
    }

    // Clean up the derived datatype
    MPI_Type_free(&MPI_MyStruct);
    MPI_Finalize();
    return 0;
} 