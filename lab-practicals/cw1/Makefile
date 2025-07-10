# Enhanced Makefile for OpenMP and MPI Coursework Programs
# 6004CEM Parallel and Distributed Programming

# Compilers and flags
CC = gcc
MPICC = mpicc
OPENMP_CFLAGS = -fopenmp -O2 -Wall
MPI_CFLAGS = -O2 -Wall
LIBS = -lm

# OpenMP Program names
OPENMP_PROGRAMS = part_a_hello_openmp part_b_vector_addition part_c_matrix_calculation \
                  part_a_hello_openmp_enhanced part_b_vector_addition_enhanced part_c_matrix_calculation_enhanced

# MPI Program names
MPI_PROGRAMS = part_a_mpi_hello_world_enhanced part_b_mpi_master_slave_enhanced mpi_part_c_tags

# All programs
ALL_PROGRAMS = $(OPENMP_PROGRAMS) $(MPI_PROGRAMS)

# Default target - build all programs
all: $(ALL_PROGRAMS)

# OpenMP Programs
part_a_hello_openmp: part_a_hello_openmp.c
	$(CC) $(OPENMP_CFLAGS) -o $@ $< $(LIBS)

part_a_hello_openmp_enhanced: part_a_hello_openmp_enhanced.c
	$(CC) $(OPENMP_CFLAGS) -o $@ $< $(LIBS)

part_b_vector_addition: part_b_vector_addition.c
	$(CC) $(OPENMP_CFLAGS) -o $@ $< $(LIBS)

part_b_vector_addition_enhanced: part_b_vector_addition_enhanced.c
	$(CC) $(OPENMP_CFLAGS) -o $@ $< $(LIBS)

part_c_matrix_calculation: part_c_matrix_calculation.c
	$(CC) $(OPENMP_CFLAGS) -o $@ $< $(LIBS)

part_c_matrix_calculation_enhanced: part_c_matrix_calculation_enhanced.c
	$(CC) $(OPENMP_CFLAGS) -o $@ $< $(LIBS)

# MPI Programs
part_a_mpi_hello_world_enhanced: part_a_mpi_hello_world_enhanced.c
	$(MPICC) $(MPI_CFLAGS) -o $@ $< $(LIBS)

part_b_mpi_master_slave_enhanced: part_b_mpi_master_slave_enhanced.c
	$(MPICC) $(MPI_CFLAGS) -o $@ $< $(LIBS)

mpi_part_c_tags: mpi_part_c_tags.c
	$(MPICC) $(MPI_CFLAGS) -o $@ $< $(LIBS)

# Separate build targets
openmp: $(OPENMP_PROGRAMS)

mpi: $(MPI_PROGRAMS)

# Clean up compiled files
clean:
	rm -f $(ALL_PROGRAMS) *.o

# Test targets
test-openmp: $(OPENMP_PROGRAMS)
	@echo "=== Testing OpenMP Programs ==="
	@echo "Testing Part A (Enhanced):"
	@echo "4" | ./part_a_hello_openmp_enhanced
	@echo "\nTesting Part B (Enhanced):"
	@./part_b_vector_addition_enhanced
	@echo "\nTesting Part C (Enhanced):"
	@./part_c_matrix_calculation_enhanced

test-mpi: $(MPI_PROGRAMS)
	@echo "=== Testing MPI Programs ==="
	@echo "Testing MPI Part A:"
	@echo "4" | mpirun -np 4 ./part_a_mpi_hello_world_enhanced
	@echo "\nTesting MPI Part B:"
	@mpirun -np 4 ./part_b_mpi_master_slave_enhanced
	@echo "\nTesting MPI Part C:"
	@mpirun -np 4 ./mpi_part_c_tags

# Individual run targets for OpenMP
run-openmp-a: part_a_hello_openmp_enhanced
	echo "8" | ./part_a_hello_openmp_enhanced

run-openmp-b: part_b_vector_addition_enhanced
	./part_b_vector_addition_enhanced

run-openmp-c: part_c_matrix_calculation_enhanced
	./part_c_matrix_calculation_enhanced

# Individual run targets for MPI
run-mpi-a: part_a_mpi_hello_world_enhanced
	echo "4" | mpirun -np 4 ./part_a_mpi_hello_world_enhanced

run-mpi-b: part_b_mpi_master_slave_enhanced
	mpirun -np 4 ./part_b_mpi_master_slave_enhanced

run-mpi-c: mpi_part_c_tags
	mpirun -np 4 ./mpi_part_c_tags

# Help target
help:
	@echo "Available targets:"
	@echo "  all           - Build all programs (OpenMP + MPI)"
	@echo "  openmp        - Build only OpenMP programs"
	@echo "  mpi           - Build only MPI programs"
	@echo "  clean         - Remove compiled files"
	@echo "  test-openmp   - Build and test OpenMP programs"
	@echo "  test-mpi      - Build and test MPI programs"
	@echo ""
	@echo "Individual OpenMP targets:"
	@echo "  run-openmp-a  - Run OpenMP Part A (enhanced)"
	@echo "  run-openmp-b  - Run OpenMP Part B (enhanced)"
	@echo "  run-openmp-c  - Run OpenMP Part C (enhanced)"
	@echo ""
	@echo "Individual MPI targets:"
	@echo "  run-mpi-a     - Run MPI Part A (4 processes)"
	@echo "  run-mpi-b     - Run MPI Part B (4 processes)"
	@echo "  run-mpi-c     - Run MPI Part C (4 processes)"
	@echo ""
	@echo "Requirements:"
	@echo "  - OpenMP: gcc with -fopenmp support"
	@echo "  - MPI: mpicc compiler and mpirun launcher"

.PHONY: all clean openmp mpi test-openmp test-mpi help \
        run-openmp-a run-openmp-b run-openmp-c \
        run-mpi-a run-mpi-b run-mpi-c 