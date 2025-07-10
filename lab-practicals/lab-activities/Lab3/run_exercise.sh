#!/bin/bash

# Script to compile and run the OpenMP nowait exercise

# Define source file and executable name
SOURCE_FILE="nowait_exercise.c"
EXECUTABLE_NAME="nowait_exercise"

# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"

# Change to the script's directory to ensure files are found
cd "$SCRIPT_DIR" || exit 1

# Clean up previous executable if it exists
if [ -f "$EXECUTABLE_NAME" ]; then
  echo "Removing previous executable: $EXECUTABLE_NAME"
  rm "$EXECUTABLE_NAME"
fi

# Compile the C code with OpenMP support
# -fopenmp: Enable OpenMP support
# -lm: Link the math library (for sqrtf)
# -o: Specify the output executable name
# -Wall -Wextra -pedantic: Enable common warnings for good practice
echo "Compiling $SOURCE_FILE with gcc and OpenMP flags..."
gcc "$SOURCE_FILE" -o "$EXECUTABLE_NAME" -fopenmp -lm -Wall -Wextra -pedantic

# Check if compilation was successful
if [ $? -ne 0 ]; then
  echo "Compilation failed!"
  exit 1
else
  echo "Compilation successful."
fi

# Run the executable
echo "Running $EXECUTABLE_NAME..."
# Set the number of threads using an environment variable (optional, overrides omp_set_num_threads if set)
# export OMP_NUM_THREADS=4

./"$EXECUTABLE_NAME"

# Check the exit status of the program
if [ $? -ne 0 ]; then
  echo "Program execution encountered an error."
  exit 1
else
  echo "Program finished."
fi

# Optional: Clean up the executable after running
# echo "Cleaning up executable: $EXECUTABLE_NAME"
# rm "$EXECUTABLE_NAME"

exit 0

