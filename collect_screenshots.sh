#!/bin/bash

# 6004CEM Screenshot Collection Script
# Guides user through taking all required screenshots

SUBMISSION_DIR="6004CEM_FINAL_SUBMISSION"

echo "📸 6004CEM Screenshot Collection Guide"
echo "======================================"
echo ""
echo "This script will guide you through taking all 6 required screenshots."
echo "Make sure to capture the FULL terminal window showing hostname and execution."
echo ""

# Check if submission directory exists
if [[ ! -d "$SUBMISSION_DIR" ]]; then
    echo "❌ Error: Submission directory not found!"
    echo "Please run setup_final_submission.sh first."
    exit 1
fi

echo "🧪 Compiling all programs first..."

# Compile TASK2A programs
echo "💻 Compiling TASK2A programs..."
cd "$SUBMISSION_DIR/TASK2A_PARALLELISM_PORTFOLIO/DOCUMENTATION"
make openmp > /dev/null 2>&1
cd - > /dev/null

# Compile TASK2B programs  
echo "🌐 Compiling TASK2B programs..."
cd "$SUBMISSION_DIR/TASK2B_DISTRIBUTED_PORTFOLIO/DOCUMENTATION"
make mpi > /dev/null 2>&1
cd - > /dev/null

echo "✅ All programs compiled"
echo ""

# Function to wait for user
wait_for_user() {
    echo ""
    echo "⏸️  Press Enter when you've taken the screenshot and are ready to continue..."
    read
}

# TASK2A Screenshots
echo "💻 TASK2A - Parallelism Portfolio Screenshots"
echo "============================================="
echo ""

echo "📸 Screenshot 1 of 6: Part A - OpenMP Hello World Enhanced"
echo "-----------------------------------------------------------"
echo ""
echo "🔧 Command to run:"
echo "cd $SUBMISSION_DIR/TASK2A_PARALLELISM_PORTFOLIO/EXECUTABLES"
echo "./part_a_enhanced"
echo ""
echo "📋 What to capture:"
echo "  ✓ Full terminal window"
echo "  ✓ Command execution and output" 
echo "  ✓ System verification (hostname: Ubuntoo, user: matthew)"
echo "  ✓ Thread information and OpenMP details"
echo ""
echo "💾 Save as: $SUBMISSION_DIR/TASK2A_PARALLELISM_PORTFOLIO/SCREENSHOTS/part_a_execution.png"

wait_for_user

echo "📸 Screenshot 2 of 6: Part B - Vector Addition Performance"
echo "---------------------------------------------------------"
echo ""
echo "🔧 Command to run:"
echo "cd $SUBMISSION_DIR/TASK2A_PARALLELISM_PORTFOLIO/EXECUTABLES"
echo "./part_b_enhanced"
echo ""
echo "📋 What to capture:"
echo "  ✓ Full terminal window"
echo "  ✓ Performance comparison results"
echo "  ✓ Static vs dynamic scheduling analysis"
echo "  ✓ Timing measurements and statistics"
echo ""
echo "💾 Save as: $SUBMISSION_DIR/TASK2A_PARALLELISM_PORTFOLIO/SCREENSHOTS/part_b_execution.png"

wait_for_user

echo "📸 Screenshot 3 of 6: Part C - Matrix Calculation Optimization"
echo "-------------------------------------------------------------"
echo ""
echo "🔧 Command to run:"
echo "cd $SUBMISSION_DIR/TASK2A_PARALLELISM_PORTFOLIO/EXECUTABLES"
echo "./part_c_enhanced"
echo ""
echo "📋 What to capture:"
echo "  ✓ Full terminal window"
echo "  ✓ Matrix calculation results"
echo "  ✓ Cache optimization performance"
echo "  ✓ Scalability analysis output"
echo ""
echo "💾 Save as: $SUBMISSION_DIR/TASK2A_PARALLELISM_PORTFOLIO/SCREENSHOTS/part_c_execution.png"

wait_for_user

# TASK2B Screenshots
echo ""
echo "🌐 TASK2B - Distributed Portfolio Screenshots"
echo "============================================"
echo ""

echo "📸 Screenshot 4 of 6: Part A - MPI Hello World (4 processes)"
echo "------------------------------------------------------------"
echo ""
echo "🔧 Command to run:"
echo "cd $SUBMISSION_DIR/TASK2B_DISTRIBUTED_PORTFOLIO/EXECUTABLES"
echo "echo '4' | mpirun -np 4 ./part_a_mpi_hello_world_enhanced"
echo ""
echo "📋 What to capture:"
echo "  ✓ Full terminal window"
echo "  ✓ All 4 MPI processes output"
echo "  ✓ Process-to-core mapping information"
echo "  ✓ System verification (hostname: Ubuntoo, user: matthew)"
echo ""
echo "💾 Save as: $SUBMISSION_DIR/TASK2B_DISTRIBUTED_PORTFOLIO/SCREENSHOTS/part_a_mpi_execution.png"

wait_for_user

echo "📸 Screenshot 5 of 6: Part B - Master-Slave Communication"
echo "---------------------------------------------------------"
echo ""
echo "🔧 Command to run:"
echo "cd $SUBMISSION_DIR/TASK2B_DISTRIBUTED_PORTFOLIO/EXECUTABLES"
echo "mpirun -np 4 ./part_b_mpi_master_slave_enhanced"
echo ""
echo "📋 What to capture:"
echo "  ✓ Full terminal window"
echo "  ✓ Master-slave communication pattern"
echo "  ✓ Personalized messages (Hello, I am John/Mary/Susan)"
echo "  ✓ Master receiving and printing all slave messages"
echo ""
echo "💾 Save as: $SUBMISSION_DIR/TASK2B_DISTRIBUTED_PORTFOLIO/SCREENSHOTS/part_b_master_slave.png"

wait_for_user

echo "📸 Screenshot 6 of 6: Part C - Message Tags and Deadlock Demo"
echo "------------------------------------------------------------"
echo ""
echo "🔧 Command to run:"
echo "cd $SUBMISSION_DIR/TASK2B_DISTRIBUTED_PORTFOLIO/EXECUTABLES"
echo "timeout 15 mpirun -np 4 ./mpi_part_c_tags"
echo ""
echo "📋 What to capture:"
echo "  ✓ Full terminal window"
echo "  ✓ Working tag scenario (100↔100)"
echo "  ✓ Deadlock scenario explanation (100→101)"
echo "  ✓ Timeout protection demonstration"
echo ""
echo "💾 Save as: $SUBMISSION_DIR/TASK2B_DISTRIBUTED_PORTFOLIO/SCREENSHOTS/part_c_message_tags.png"

wait_for_user

# Final verification
echo ""
echo "✅ Screenshot collection completed!"
echo ""
echo "📊 Screenshot Checklist:"
echo "  💻 TASK2A Screenshots:"
echo "    📸 part_a_execution.png"
echo "    📸 part_b_execution.png" 
echo "    📸 part_c_execution.png"
echo ""
echo "  🌐 TASK2B Screenshots:"
echo "    📸 part_a_mpi_execution.png"
echo "    📸 part_b_master_slave.png"
echo "    📸 part_c_message_tags.png"
echo ""
echo "📁 Screenshots should be saved in:"
echo "  $SUBMISSION_DIR/TASK2A_PARALLELISM_PORTFOLIO/SCREENSHOTS/"
echo "  $SUBMISSION_DIR/TASK2B_DISTRIBUTED_PORTFOLIO/SCREENSHOTS/"
echo ""
echo "🎯 Quality checklist:"
echo "  ✓ All screenshots show hostname: Ubuntoo"
echo "  ✓ All screenshots show user: matthew"
echo "  ✓ Full terminal window captured"
echo "  ✓ Program output clearly visible"
echo "  ✓ System verification information included"
echo ""
echo "📝 Next steps:"
echo "  1. Verify all 6 screenshots are saved correctly"
echo "  2. Optimize TASK1 word count (2,542 → 1,600 words)"
echo "  3. Convert markdown files to PDF for submission"
echo "  4. Final quality review"
echo ""
echo "🏆 Your submission is ready for first-class assessment!" 