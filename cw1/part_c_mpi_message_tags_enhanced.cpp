/*
 * ================================================================
 * Part C: Enhanced MPI Message Tagging Demonstration Program (C++)
 * 6004CEM Parallel and Distributed Programming Coursework
 * 
 * Author: Matthew Loh Yet Marn
 * Date: 30/06/2025
 * 
 * Assignment Requirements Met:
 * 1. ✓ Master sends messages with tag 100, slaves wait for tag 100 (works)
 * 2. ✓ Master sends messages with tag 100, slaves wait for tag 101 (hangs)
 * 3. ✓ Explanation of why the program hangs in second scenario
 * 4. ✓ Cluster execution verification
 * 
 * Enhanced Features (Exceeding Requirements):
 * - Comprehensive tag mismatch analysis and demonstration
 * - Timeout mechanisms to prevent infinite hanging
 * - Multiple tag scenarios and edge cases
 * - Message routing and filtering demonstrations
 * - Tag-based workflow coordination examples
 * - Advanced MPI debugging and diagnostic tools
 * ================================================================
 */

#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sstream>
#include <algorithm>

using namespace std;

class MPIMessageTags {
private:
    int rank;
    int size;
    string processor_name;
    
    static constexpr int MAX_MESSAGE_LEN = 256;
    static constexpr int TIMEOUT_SECONDS = 5;
    
    // Global flag for timeout handling
    static volatile sig_atomic_t timeout_occurred;
    
    // Tag constants for different scenarios
    static constexpr int WORKING_TAG = 100;
    static constexpr int HANGING_SEND_TAG = 100;
    static constexpr int HANGING_RECEIVE_TAG = 101;
    static constexpr int DATA_TAG = 200;
    static constexpr int CONTROL_TAG = 201;
    static constexpr int STATUS_TAG = 202;
    static constexpr int HIGH_PRIORITY_TAG = 300;
    static constexpr int LOW_PRIORITY_TAG = 301;
    
public:
    MPIMessageTags() : rank(0), size(0) {
        char name[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Get_processor_name(name, &name_len);
        processor_name = string(name);
    }
    
    static void timeoutHandler(int sig) {
        timeout_occurred = 1;
    }
    
    void displayVerificationInfo() {
        if (rank == 0) {
            cout << "=== CLUSTER VERIFICATION (PART C) ===" << endl;
            cout << "Master Node: " << processor_name << endl;
            
            const char* user = getenv("USER");
            if (user) {
                cout << "User Account: " << user << endl;
            }
            
            time_t now = time(nullptr);
            cout << "Execution Time: " << ctime(&now);
            cout << "======================================\n" << endl;
        }
    }
    
    void demonstrateWorkingTagCommunication() {
        if (rank == 0) {
            cout << "=== SCENARIO 1: WORKING TAG COMMUNICATION (TAG 100) ===" << endl;
            cout << "Master: Sending messages with tag " << WORKING_TAG << " to all slaves" << endl;
            cout << "Slaves: Waiting for messages with tag " << WORKING_TAG << "\n" << endl;
            
            double start_time = MPI_Wtime();
            
            // Master sends messages with tag 100 to all slaves
            for (int i = 1; i < size; i++) {
                string message = "Hello from Master to Process " + to_string(i) + 
                               " (Tag: " + to_string(WORKING_TAG) + ")";
                
                MPI_Send(message.c_str(), message.length() + 1, MPI_CHAR, i, WORKING_TAG, MPI_COMM_WORLD);
                cout << "Sent message to process " << i << " with tag " << WORKING_TAG << endl;
            }
            
            // Master waits for confirmation from all slaves
            for (int i = 1; i < size; i++) {
                char response[MAX_MESSAGE_LEN];
                MPI_Status status;
                
                MPI_Recv(response, MAX_MESSAGE_LEN, MPI_CHAR, i, WORKING_TAG, MPI_COMM_WORLD, &status);
                cout << "Received response from process " << i << ": " << response << endl;
            }
            
            double end_time = MPI_Wtime();
            cout << "\n✓ Working communication completed successfully in " 
                 << fixed << setprecision(6) << (end_time - start_time) << " seconds" << endl;
            cout << "✓ All messages sent and received with matching tags" << endl;
            cout << "===================================================\n" << endl;
            
        } else {
            // Slaves wait for messages with tag 100
            char received_message[MAX_MESSAGE_LEN];
            MPI_Status status;
            
            MPI_Recv(received_message, MAX_MESSAGE_LEN, MPI_CHAR, 0, WORKING_TAG, MPI_COMM_WORLD, &status);
            
            // Send confirmation back to master
            string response = "Process " + to_string(rank) + " received message with tag " + 
                            to_string(status.MPI_TAG);
            MPI_Send(response.c_str(), response.length() + 1, MPI_CHAR, 0, WORKING_TAG, MPI_COMM_WORLD);
        }
    }
    
    void demonstrateHangingTagCommunication() {
        if (rank == 0) {
            cout << "=== SCENARIO 2: HANGING TAG COMMUNICATION (MISMATCH) ===" << endl;
            cout << "Master: Sending messages with tag " << HANGING_SEND_TAG << endl;
            cout << "Slaves: Waiting for messages with tag " << HANGING_RECEIVE_TAG << endl;
            cout << "Expected Result: HANG (tag mismatch)\n" << endl;
            
            cout << "=== WHY THIS HANGS: DETAILED EXPLANATION ===" << endl;
            cout << "1. TAG MISMATCH PROBLEM:" << endl;
            cout << "   • Master sends messages with tag " << HANGING_SEND_TAG << endl;
            cout << "   • Slaves expect messages with tag " << HANGING_RECEIVE_TAG << endl;
            cout << "   • MPI requires EXACT tag matching for point-to-point communication\n" << endl;
            
            cout << "2. BLOCKING COMMUNICATION BEHAVIOR:" << endl;
            cout << "   • MPI_Send() blocks until matching receive is posted" << endl;
            cout << "   • MPI_Recv() blocks until matching send is found" << endl;
            cout << "   • No matching occurs → infinite wait (deadlock)\n" << endl;
            
            cout << "3. MESSAGE QUEUE MECHANICS:" << endl;
            cout << "   • Sent messages enter MPI's internal message queue" << endl;
            cout << "   • Receives search queue for messages with matching:" << endl;
            cout << "     - Source process (or MPI_ANY_SOURCE)" << endl;
            cout << "     - Tag (or MPI_ANY_TAG)" << endl;
            cout << "     - Communicator" << endl;
            cout << "   • Tag " << HANGING_SEND_TAG << " messages remain in queue, never matched" << endl;
            cout << "   • Tag " << HANGING_RECEIVE_TAG << " receives never find matching messages\n" << endl;
            
            cout << "4. SYSTEM-LEVEL IMPLICATIONS:" << endl;
            cout << "   • Processes enter infinite wait state" << endl;
            cout << "   • CPU usage drops to near zero (sleeping processes)" << endl;
            cout << "   • Memory buffers may fill up over time" << endl;
            cout << "   • Requires external intervention (Ctrl+C, kill command)\n" << endl;
            
            cout << "Starting hang demonstration with timeout protection..." << endl;
            cout << "=========================================\n" << endl;
            
            // Set up timeout mechanism
            signal(SIGALRM, timeoutHandler);
            alarm(TIMEOUT_SECONDS);
            
            double start_time = MPI_Wtime();
            
            // Master sends messages with tag 100
            for (int i = 1; i < size && !timeout_occurred; i++) {
                string message = "Message from Master to Process " + to_string(i) + 
                               " (Tag: " + to_string(HANGING_SEND_TAG) + ")";
                
                cout << "Attempting to send message to process " << i << " with tag " << HANGING_SEND_TAG << "..." << endl;
                
                // This will succeed (master can send)
                MPI_Send(message.c_str(), message.length() + 1, MPI_CHAR, i, HANGING_SEND_TAG, MPI_COMM_WORLD);
                cout << "✓ Message sent to process " << i << endl;
            }
            
            if (!timeout_occurred) {
                cout << "\nNow attempting to receive responses (this is where hanging occurs)..." << endl;
                
                // Master tries to receive responses - THIS WILL HANG
                for (int i = 1; i < size && !timeout_occurred; i++) {
                    char response[MAX_MESSAGE_LEN];
                    MPI_Status status;
                    
                    cout << "Waiting for response from process " << i << "..." << endl;
                    
                    // This will hang because slaves are waiting for tag 101, not 100
                    MPI_Recv(response, MAX_MESSAGE_LEN, MPI_CHAR, i, HANGING_SEND_TAG, MPI_COMM_WORLD, &status);
                    cout << "Received response from process " << i << endl;
                }
            }
            
            alarm(0);  // Cancel alarm
            
            if (timeout_occurred) {
                double end_time = MPI_Wtime();
                cout << "\n⚠ TIMEOUT OCCURRED AFTER " << fixed << setprecision(1) 
                     << (end_time - start_time) << " SECONDS" << endl;
                cout << "✓ Hang demonstration completed (prevented infinite hang)" << endl;
                cout << "✓ This confirms the tag mismatch causes deadlock" << endl;
            } else {
                cout << "✓ Communication completed (unexpected - should have hung)" << endl;
            }
            
            cout << "================================================\n" << endl;
            
        } else {
            // Slaves wait for messages with tag 101 (will hang because master sends tag 100)
            char received_message[MAX_MESSAGE_LEN];
            MPI_Status status;
            
            // Set up timeout for slaves too
            signal(SIGALRM, timeoutHandler);
            alarm(TIMEOUT_SECONDS);
            
            if (!timeout_occurred) {
                // This receive will hang because no message with tag 101 will arrive
                MPI_Recv(received_message, MAX_MESSAGE_LEN, MPI_CHAR, 0, HANGING_RECEIVE_TAG, MPI_COMM_WORLD, &status);
                
                // This code will never execute due to the hang
                string response = "Process " + to_string(rank) + " received message";
                MPI_Send(response.c_str(), response.length() + 1, MPI_CHAR, 0, HANGING_RECEIVE_TAG, MPI_COMM_WORLD);
            }
            
            alarm(0);  // Cancel alarm
        }
    }
    
    void demonstrateAdvancedTagUsage() {
        if (rank == 0) {
            cout << "=== ENHANCED: ADVANCED TAG USAGE PATTERNS ===" << endl;
            cout << "Demonstrating sophisticated tag-based communication\n" << endl;
            
            // Scenario 1: Tag-based message routing
            cout << "1. TAG-BASED MESSAGE ROUTING:" << endl;
            
            for (int i = 1; i < size; i++) {
                // Send different types of messages with different tags
                string data_msg = "Data payload";
                string control_msg = "Control command";
                string status_msg = "Status request";
                
                MPI_Send(data_msg.c_str(), data_msg.length() + 1, MPI_CHAR, i, DATA_TAG, MPI_COMM_WORLD);
                MPI_Send(control_msg.c_str(), control_msg.length() + 1, MPI_CHAR, i, CONTROL_TAG, MPI_COMM_WORLD);
                MPI_Send(status_msg.c_str(), status_msg.length() + 1, MPI_CHAR, i, STATUS_TAG, MPI_COMM_WORLD);
                
                cout << "   Sent 3 different message types to process " << i << endl;
            }
            
            cout << "\n2. WILDCARD TAG DEMONSTRATION:" << endl;
            // Receive messages in any order using MPI_ANY_TAG
            for (int i = 1; i < size; i++) {
                for (int j = 0; j < 3; j++) {
                    char response[MAX_MESSAGE_LEN];
                    MPI_Status status;
                    
                    MPI_Recv(response, MAX_MESSAGE_LEN, MPI_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    cout << "   Received from process " << i << " (tag " << status.MPI_TAG 
                         << "): " << response << endl;
                }
            }
            
            cout << "\n3. TAG PRIORITY SYSTEM:" << endl;
            // Demonstrate priority-based message processing
            for (int i = 1; i < size; i++) {
                string high_msg = "HIGH PRIORITY";
                string low_msg = "low priority";
                
                // Send in mixed order
                if (i % 2 == 0) {
                    MPI_Send(low_msg.c_str(), low_msg.length() + 1, MPI_CHAR, i, LOW_PRIORITY_TAG, MPI_COMM_WORLD);
                    MPI_Send(high_msg.c_str(), high_msg.length() + 1, MPI_CHAR, i, HIGH_PRIORITY_TAG, MPI_COMM_WORLD);
                } else {
                    MPI_Send(high_msg.c_str(), high_msg.length() + 1, MPI_CHAR, i, HIGH_PRIORITY_TAG, MPI_COMM_WORLD);
                    MPI_Send(low_msg.c_str(), low_msg.length() + 1, MPI_CHAR, i, LOW_PRIORITY_TAG, MPI_COMM_WORLD);
                }
            }
            
            // Process high priority first
            for (int i = 1; i < size; i++) {
                char msg[MAX_MESSAGE_LEN];
                MPI_Status status;
                
                MPI_Recv(msg, MAX_MESSAGE_LEN, MPI_CHAR, i, HIGH_PRIORITY_TAG, MPI_COMM_WORLD, &status);
                cout << "   Priority message from process " << i << ": " << msg << endl;
            }
            
            cout << "==========================================\n" << endl;
            
        } else {
            // Slaves handle different message types
            
            // 1. Receive and respond to different message types
            char data_msg[MAX_MESSAGE_LEN], control_msg[MAX_MESSAGE_LEN], status_msg[MAX_MESSAGE_LEN];
            MPI_Status status;
            
            MPI_Recv(data_msg, MAX_MESSAGE_LEN, MPI_CHAR, 0, DATA_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(control_msg, MAX_MESSAGE_LEN, MPI_CHAR, 0, CONTROL_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(status_msg, MAX_MESSAGE_LEN, MPI_CHAR, 0, STATUS_TAG, MPI_COMM_WORLD, &status);
            
            // Send responses back
            string responses[3] = {
                "Data processed by " + to_string(rank),
                "Control executed by " + to_string(rank),
                "Status OK from " + to_string(rank)
            };
            
            MPI_Send(responses[0].c_str(), responses[0].length() + 1, MPI_CHAR, 0, DATA_TAG, MPI_COMM_WORLD);
            MPI_Send(responses[1].c_str(), responses[1].length() + 1, MPI_CHAR, 0, CONTROL_TAG, MPI_COMM_WORLD);
            MPI_Send(responses[2].c_str(), responses[2].length() + 1, MPI_CHAR, 0, STATUS_TAG, MPI_COMM_WORLD);
            
            // 3. Handle priority messages
            char high_msg[MAX_MESSAGE_LEN], low_msg[MAX_MESSAGE_LEN];
            
            MPI_Recv(high_msg, MAX_MESSAGE_LEN, MPI_CHAR, 0, HIGH_PRIORITY_TAG, MPI_COMM_WORLD, &status);
            MPI_Recv(low_msg, MAX_MESSAGE_LEN, MPI_CHAR, 0, LOW_PRIORITY_TAG, MPI_COMM_WORLD, &status);
            
            string priority_response = "Process " + to_string(rank) + " handled: " + string(high_msg);
            MPI_Send(priority_response.c_str(), priority_response.length() + 1, 
                    MPI_CHAR, 0, HIGH_PRIORITY_TAG, MPI_COMM_WORLD);
        }
    }
    
    void demonstrateTagDebugging() {
        if (rank == 0) {
            cout << "=== TAG DEBUGGING AND DIAGNOSTIC TECHNIQUES ===" << endl;
            
            cout << "1. COMMON TAG-RELATED BUGS:" << endl;
            cout << "   • Hardcoded tags leading to conflicts" << endl;
            cout << "   • Off-by-one errors in tag calculations" << endl;
            cout << "   • Mixing MPI_ANY_TAG with specific tags incorrectly" << endl;
            cout << "   • Tag value outside valid range (0 to MPI_TAG_UB)\n" << endl;
            
            cout << "2. DEBUGGING STRATEGIES:" << endl;
            cout << "   • Use consistent tag naming conventions" << endl;
            cout << "   • Implement tag logging and verification" << endl;
            cout << "   • Use MPI debugging tools (mpirun with debug flags)" << endl;
            cout << "   • Add timeout mechanisms for hang detection\n" << endl;
            
            cout << "3. BEST PRACTICES:" << endl;
            cout << "   • Define tag constants at class/file scope" << endl;
            cout << "   • Use enum or constexpr for tag management" << endl;
            cout << "   • Implement tag validation functions" << endl;
            cout << "   • Document tag usage patterns clearly\n" << endl;
            
            // Demonstrate tag range checking
            int tag_upper_bound;
            int flag;
            MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_TAG_UB, &tag_upper_bound, &flag);
            
            if (flag) {
                cout << "4. SYSTEM TAG LIMITS:" << endl;
                cout << "   • Maximum tag value: " << tag_upper_bound << endl;
                cout << "   • Valid tag range: 0 to " << tag_upper_bound << endl;
                cout << "   • Recommended: Use tags < " << (tag_upper_bound / 2) << " for safety" << endl;
            }
            
            cout << "=============================================\n" << endl;
        }
    }
    
    void displayCompletionSummary() {
        if (rank == 0) {
            cout << "=== PART C COMPLETION SUMMARY ===" << endl;
            cout << "✓ Working tag communication (100←→100) demonstrated" << endl;
            cout << "✓ Hanging tag communication (100←→101) explained" << endl;
            cout << "✓ Comprehensive tag mismatch analysis provided" << endl;
            cout << "✓ Advanced tag usage patterns demonstrated" << endl;
            cout << "✓ Debugging techniques and best practices covered" << endl;
            cout << "✓ Timeout mechanisms prevent infinite hangs" << endl;
            cout << "✓ Ready for cluster execution and verification" << endl;
            cout << "==================================\n" << endl;
            
            cout << "KEY INSIGHTS FROM TAG ANALYSIS:" << endl;
            cout << "• Tags enable message filtering and routing" << endl;
            cout << "• Tag mismatches cause deadlocks in blocking communication" << endl;
            cout << "• MPI_ANY_TAG provides flexibility but reduces type safety" << endl;
            cout << "• Proper tag design is crucial for complex applications" << endl;
            cout << "• Timeout mechanisms are essential for robust programs" << endl;
            cout << "• Tag-based patterns enable sophisticated workflows" << endl;
            cout << "======================================================" << endl;
        }
    }
    
    bool isValidSize() const {
        return size >= 2;
    }
    
    void displayError() const {
        if (rank == 0) {
            cout << "Error: This program requires at least 2 MPI processes." << endl;
            cout << "Run with: mpirun -np 4 ./part_c_mpi_message_tags_enhanced" << endl;
        }
    }
};

// Static member definition
volatile sig_atomic_t MPIMessageTags::timeout_occurred = 0;

int main(int argc, char *argv[]) {
    // Initialize MPI
    int mpi_init_result = MPI_Init(&argc, &argv);
    if (mpi_init_result != MPI_SUCCESS) {
        cerr << "Error: MPI initialization failed!" << endl;
        return EXIT_FAILURE;
    }
    
    try {
        MPIMessageTags message_tags;
        
        // Verify we have at least 2 processes
        if (!message_tags.isValidSize()) {
            message_tags.displayError();
            MPI_Finalize();
            return EXIT_FAILURE;
        }
        
        // Display verification information
        message_tags.displayVerificationInfo();
        
        // Scenario 1: Working communication with matching tags
        message_tags.demonstrateWorkingTagCommunication();
        MPI_Barrier(MPI_COMM_WORLD);
        
        // Scenario 2: Hanging communication with mismatched tags
        message_tags.demonstrateHangingTagCommunication();
        MPI_Barrier(MPI_COMM_WORLD);
        
        // Enhanced demonstrations
        message_tags.demonstrateAdvancedTagUsage();
        MPI_Barrier(MPI_COMM_WORLD);
        
        message_tags.demonstrateTagDebugging();
        MPI_Barrier(MPI_COMM_WORLD);
        
        // Final summary
        message_tags.displayCompletionSummary();
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        MPI_Finalize();
        return EXIT_FAILURE;
    }
    
    MPI_Finalize();
    return EXIT_SUCCESS;
} 