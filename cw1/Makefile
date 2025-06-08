# Makefile for OpenMP Coursework Programs
# Compiler and flags
CC = gcc
CFLAGS = -fopenmp -O2 -Wall
LIBS = -lm

# Program names
PROGRAMS = part_a_hello_openmp part_b_vector_addition part_c_matrix_calculation

# Default target - build all programs
all: $(PROGRAMS)

# Part A: Hello World
part_a_hello_openmp: part_a_hello_openmp.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Part B: Vector Addition
part_b_vector_addition: part_b_vector_addition.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Part C: Matrix Calculation
part_c_matrix_calculation: part_c_matrix_calculation.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

# Clean up compiled files
clean:
	rm -f $(PROGRAMS) *.o

# Run all programs (for testing)
run-all: all
	@echo "=== Running Part A ==="
	echo "10" | ./part_a_hello_openmp
	@echo "\n=== Running Part B ==="
	./part_b_vector_addition
	@echo "\n=== Running Part C ==="
	./part_c_matrix_calculation

# Individual run targets
run-a: part_a_hello_openmp
	echo "8" | ./part_a_hello_openmp

run-b: part_b_vector_addition
	./part_b_vector_addition

run-c: part_c_matrix_calculation
	./part_c_matrix_calculation

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build all programs"
	@echo "  clean      - Remove compiled files"
	@echo "  run-all    - Build and run all programs"
	@echo "  run-a      - Build and run Part A"
	@echo "  run-b      - Build and run Part B"
	@echo "  run-c      - Build and run Part C"
	@echo "  help       - Show this help message"

.PHONY: all clean run-all run-a run-b run-c help 