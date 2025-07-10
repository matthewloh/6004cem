#!/bin/bash

# Set the C++ source file and executable name
SOURCE_FILE="lab_activity_e_seq.cpp"
EXECUTABLE_NAME="matrix_multiply"
OUTPUT_FILE="results.csv"

# Compile the C++ code with OpenMP and optimization
# Using -O3 for potentially better optimization
echo "Compiling ${SOURCE_FILE}..."
g++ -fopenmp -O3 "${SOURCE_FILE}" -o "${EXECUTABLE_NAME}" -lm

# Check if compilation was successful
if [ $? -ne 0 ]; then
  echo "Compilation failed!"
  exit 1
fi

echo "Compilation successful."

# Run the executable and redirect output to the CSV file
echo "Running experiment and saving results to ${OUTPUT_FILE}..."
./"${EXECUTABLE_NAME}" > "${OUTPUT_FILE}"

# Check if the executable ran successfully
if [ $? -ne 0 ]; then
  echo "Execution failed!"
  exit 1
fi

echo "Experiment finished. Results saved to ${OUTPUT_FILE}."

exit 0 