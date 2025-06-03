#!/bin/bash

echo "=== Compiling and Running Lab 8 Examples ==="
echo

# Compile all examples
echo "1. Compiling OpenMP example..."
mpicc -fopenmp l/8a.c -o l/8a
if [ $? -eq 0 ]; then
  echo "✓ OpenMP compilation successful"
else
  echo "✗ OpenMP compilation failed"
  exit 1
fi

echo "2. Compiling MPI example..."
mpicc l/8b.c -o l/8b
if [ $? -eq 0 ]; then
  echo "✓ MPI compilation successful"
else
  echo "✗ MPI compilation failed"
  exit 1
fi

echo "3. Compiling Hybrid MPI+OpenMP example..."
mpicc -fopenmp l/8c.c -o l/8c
if [ $? -eq 0 ]; then
  echo "✓ Hybrid compilation successful"
else
  echo "✗ Hybrid compilation failed"
  exit 1
fi

echo
echo "=== Running Examples ==="
echo

# Run OpenMP example
echo "Running Example A (OpenMP):"
mpiexec l/8a
echo

# Run MPI example with 4 processes
echo "Running Example B (MPI with 4 processes):"
mpiexec -np 4 l/8b
echo

# Run Hybrid example with 4 processes
echo "Running Example C (Hybrid MPI+OpenMP with 4 processes):"
mpiexec -np 4 l/8c
echo

echo "=== All examples completed! ==="
