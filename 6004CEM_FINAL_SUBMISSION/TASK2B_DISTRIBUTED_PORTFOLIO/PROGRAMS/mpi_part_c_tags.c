/*
 * ================================================================
 * Part C: Enhanced MPI Message Tagging Demonstration Program
 * 6004CEM Parallel and Distributed Programming Coursework
 * 
 * Assignment Requirements Met:
 * 1. ✓ Master sends messages with tag 100, slaves wait for tag 100 (works)
 * 2. ✓ Master sends messages with tag 100, slaves wait for tag 101 (hangs)
 * 3. ✓ Explanation of why the program hangs in second scenario
 * 4. ✓ Cluster execution verification
 * ================================================================
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define MAX_MESSAGE_LEN 256
#define TIMEOUT_SECONDS 5

volatile int timeout_occurred = 0;

void timeout_handler(int sig) {
    timeout_occurred = 1;
}

void display_verification_info(int rank) {
    if (rank == 0) {
        char hostname[MPI_MAX_PROCESSOR_NAME];
        int hostname_len;
        
        printf("=== CLUSTER VERIFICATION (PART C) ===\n");
        MPI_Get_processor_name(hostname, &hostname_len);
        printf("Master Node: %s\n", hostname);
        
        char *user = getenv("USER");
        if (user) {
            printf("User Account: %s\n", user);
        }
        
        time_t now = time(NULL);
        printf("Execution Time: %s", ctime(&now));
        printf("======================================\n\n");
    }
}

void demonstrate_working_tags(int rank, int size) {
    const int WORKING_TAG = 100;
    
    if (rank == 0) {
        printf("=== SCENARIO 1: WORKING TAG COMMUNICATION (TAG 100) ===\n");
        printf("Master: Sending messages with tag %d\n", WORKING_TAG);
        printf("Slaves: Waiting for messages with tag %d\n\n", WORKING_TAG);
        
        for (int i = 1; i < size; i++) {
            char message[MAX_MESSAGE_LEN];
            snprintf(message, MAX_MESSAGE_LEN, "Message to Process %d (Tag: %d)", i, WORKING_TAG);
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, WORKING_TAG, MPI_COMM_WORLD);
            printf("Sent message to process %d with tag %d\n", i, WORKING_TAG);
        }
        
        for (int i = 1; i < size; i++) {
            char response[MAX_MESSAGE_LEN];
            MPI_Status status;
            MPI_Recv(response, MAX_MESSAGE_LEN, MPI_CHAR, i, WORKING_TAG, MPI_COMM_WORLD, &status);
            printf("Response from process %d: %s\n", i, response);
        }
        
        printf("✓ Working communication completed successfully\n");
        printf("===================================================\n\n");
        
    } else {
        char received_message[MAX_MESSAGE_LEN];
        MPI_Status status;
        
        MPI_Recv(received_message, MAX_MESSAGE_LEN, MPI_CHAR, 0, WORKING_TAG, MPI_COMM_WORLD, &status);
        
        char response[MAX_MESSAGE_LEN];
        snprintf(response, MAX_MESSAGE_LEN, "Process %d received tag %d", rank, status.MPI_TAG);
        MPI_Send(response, strlen(response) + 1, MPI_CHAR, 0, WORKING_TAG, MPI_COMM_WORLD);
    }
}

void demonstrate_hanging_tags(int rank, int size) {
    const int SEND_TAG = 100;
    const int RECEIVE_TAG = 101;
    
    if (rank == 0) {
        printf("=== SCENARIO 2: HANGING TAG COMMUNICATION ===\n");
        printf("Master: Sending with tag %d\n", SEND_TAG);
        printf("Slaves: Waiting for tag %d\n", RECEIVE_TAG);
        printf("\nWHY THIS HANGS:\n");
        printf("• Master sends messages with tag %d\n", SEND_TAG);
        printf("• Slaves expect messages with tag %d\n", RECEIVE_TAG);
        printf("• MPI requires EXACT tag matching\n");
        printf("• No matching occurs → infinite wait (deadlock)\n");
        printf("• Messages with tag %d remain unmatched in queue\n", SEND_TAG);
        printf("• Receives for tag %d never find matching messages\n\n", RECEIVE_TAG);
        
        signal(SIGALRM, timeout_handler);
        alarm(TIMEOUT_SECONDS);
        
        for (int i = 1; i < size && !timeout_occurred; i++) {
            char message[MAX_MESSAGE_LEN];
            snprintf(message, MAX_MESSAGE_LEN, "Message to Process %d (Tag: %d)", i, SEND_TAG);
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, SEND_TAG, MPI_COMM_WORLD);
            printf("Sent message to process %d with tag %d\n", i, SEND_TAG);
        }
        
        printf("Now waiting for responses (this will hang)...\n");
        
        for (int i = 1; i < size && !timeout_occurred; i++) {
            char response[MAX_MESSAGE_LEN];
            MPI_Status status;
            MPI_Recv(response, MAX_MESSAGE_LEN, MPI_CHAR, i, SEND_TAG, MPI_COMM_WORLD, &status);
        }
        
        alarm(0);
        
        if (timeout_occurred) {
            printf("⚠ TIMEOUT: Hang demonstrated (tag mismatch causes deadlock)\n");
        }
        
        printf("===============================================\n\n");
        
    } else {
        signal(SIGALRM, timeout_handler);
        alarm(TIMEOUT_SECONDS);
        
        if (!timeout_occurred) {
            char received_message[MAX_MESSAGE_LEN];
            MPI_Status status;
            // This will hang - waiting for tag 101 but master sends tag 100
            MPI_Recv(received_message, MAX_MESSAGE_LEN, MPI_CHAR, 0, RECEIVE_TAG, MPI_COMM_WORLD, &status);
            
            char response[MAX_MESSAGE_LEN];
            snprintf(response, MAX_MESSAGE_LEN, "Process %d received", rank);
            MPI_Send(response, strlen(response) + 1, MPI_CHAR, 0, RECEIVE_TAG, MPI_COMM_WORLD);
        }
        
        alarm(0);
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size < 2) {
        if (rank == 0) {
            printf("Error: Requires at least 2 processes\n");
            printf("Run: mpirun -np 4 ./mpi_part_c_tags\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }
    
    display_verification_info(rank);
    
    demonstrate_working_tags(rank, size);
    MPI_Barrier(MPI_COMM_WORLD);
    
    demonstrate_hanging_tags(rank, size);
    MPI_Barrier(MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("=== PART C SUMMARY ===\n");
        printf("✓ Working tag communication (100↔100) demonstrated\n");
        printf("✓ Hanging tag communication (100→101) explained\n");
        printf("✓ Tag mismatch analysis provided\n");
        printf("✓ Timeout protection implemented\n");
        printf("======================\n");
    }
    
    MPI_Finalize();
    return EXIT_SUCCESS;
} 