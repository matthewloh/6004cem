# 6004CEM TASK2A Parallelism Portfolio Makefile
CC = gcc
CFLAGS = -fopenmp -O3 -Wall -Wextra -std=c11
PROGRAMS_DIR = ../PROGRAMS
EXECUTABLES_DIR = ../EXECUTABLES

PART_A_SRC = $(PROGRAMS_DIR)/part_a_hello_openmp_enhanced.c
PART_B_SRC = $(PROGRAMS_DIR)/part_b_vector_addition_enhanced.c
PART_C_SRC = $(PROGRAMS_DIR)/part_c_matrix_calculation_enhanced.c

PART_A_EXE = $(EXECUTABLES_DIR)/part_a_enhanced
PART_B_EXE = $(EXECUTABLES_DIR)/part_b_enhanced
PART_C_EXE = $(EXECUTABLES_DIR)/part_c_enhanced

.PHONY: all openmp clean test-openmp

all: openmp

openmp: $(PART_A_EXE) $(PART_B_EXE) $(PART_C_EXE)

$(PART_A_EXE): $(PART_A_SRC)
	@echo "🔨 Compiling Part A: OpenMP Hello World Enhanced..."
	$(CC) $(CFLAGS) -o $@ $<

$(PART_B_EXE): $(PART_B_SRC)
	@echo "🔨 Compiling Part B: Vector Addition Enhanced..."
	$(CC) $(CFLAGS) -o $@ $< -lm

$(PART_C_EXE): $(PART_C_SRC)
	@echo "🔨 Compiling Part C: Matrix Calculation Enhanced..."
	$(CC) $(CFLAGS) -o $@ $< -lm

test-openmp: openmp
	@echo "🧪 Testing TASK2A programs..."
	cd $(EXECUTABLES_DIR) && ./part_a_enhanced && ./part_b_enhanced && ./part_c_enhanced

clean:
	rm -f $(PART_A_EXE) $(PART_B_EXE) $(PART_C_EXE)
