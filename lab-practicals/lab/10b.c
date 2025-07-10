#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    const int rows = 4, cols = 5;
    float matrix[rows][cols];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        printf("This program requires at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }

    // Create vector type for extracting a column from the matrix
    MPI_Datatype column_type;
    MPI_Type_vector(rows, 1, cols, MPI_FLOAT, &column_type);
    MPI_Type_commit(&column_type);

    // Create vector type for extracting a row from the matrix
    MPI_Datatype row_type;
    MPI_Type_contiguous(cols, MPI_FLOAT, &row_type);
    MPI_Type_commit(&row_type);

    if (rank == 0) {
        // Initialize matrix with values
        printf("Process 0 - Original matrix (%dx%d):\n", rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = i * cols + j + 1;  // Values 1-20
                printf("%6.1f ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        // Send column 2 (3rd column, 0-indexed)
        printf("Process 0 - Sending column 2 to process 1\n");
        MPI_Send(&matrix[0][2], 1, column_type, 1, 0, MPI_COMM_WORLD);

        // Send row 1 (2nd row, 0-indexed) to process 1 if we have more processes
        if (size > 2) {
            printf("Process 0 - Sending row 1 to process 2\n");
            MPI_Send(&matrix[1][0], 1, row_type, 2, 1, MPI_COMM_WORLD);
        }

    } else if (rank == 1) {
        // Receive column data
        float column_data[rows];
        MPI_Recv(column_data, rows, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process 1 - Received column 2:\n");
        for (int i = 0; i < rows; i++) {
            printf("  Row %d: %.1f\n", i, column_data[i]);
        }
        printf("\n");

    } else if (rank == 2) {
        // Receive row data
        float row_data[cols];
        MPI_Recv(row_data, cols, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process 2 - Received row 1:\n");
        printf("  ");
        for (int j = 0; j < cols; j++) {
            printf("%.1f ", row_data[j]);
        }
        printf("\n\n");
    }

    // Clean up the derived datatypes
    MPI_Type_free(&column_type);
    MPI_Type_free(&row_type);
    MPI_Finalize();
    return 0;
} 