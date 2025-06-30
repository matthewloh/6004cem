# 6004CEM TASK2B Distributed Portfolio Makefile
MPICC = mpicc
CFLAGS = -O3 -Wall -Wextra -std=c11
PROGRAMS_DIR = ../PROGRAMS
EXECUTABLES_DIR = ../EXECUTABLES

PART_A_SRC = $(PROGRAMS_DIR)/part_a_mpi_hello_world_enhanced.c
PART_B_SRC = $(PROGRAMS_DIR)/part_b_mpi_master_slave_enhanced.c
PART_C_SRC = $(PROGRAMS_DIR)/mpi_part_c_tags.c

PART_A_EXE = $(EXECUTABLES_DIR)/part_a_mpi_hello_world_enhanced
PART_B_EXE = $(EXECUTABLES_DIR)/part_b_mpi_master_slave_enhanced
PART_C_EXE = $(EXECUTABLES_DIR)/mpi_part_c_tags

.PHONY: all mpi clean test-mpi

all: mpi

mpi: $(PART_A_EXE) $(PART_B_EXE) $(PART_C_EXE)

$(PART_A_EXE): $(PART_A_SRC)
	@echo "🔨 Compiling Part A: MPI Hello World Enhanced..."
	$(MPICC) $(CFLAGS) -o $@ $<

$(PART_B_EXE): $(PART_B_SRC)
	@echo "🔨 Compiling Part B: MPI Master-Slave Enhanced..."
	$(MPICC) $(CFLAGS) -o $@ $<

$(PART_C_EXE): $(PART_C_SRC)
	@echo "🔨 Compiling Part C: MPI Message Tags..."
	$(MPICC) $(CFLAGS) -o $@ $<

test-mpi: mpi
	@echo "🧪 Testing TASK2B programs..."
	cd $(EXECUTABLES_DIR) && echo "4" | mpirun -np 4 ./part_a_mpi_hello_world_enhanced
	cd $(EXECUTABLES_DIR) && mpirun -np 4 ./part_b_mpi_master_slave_enhanced
	cd $(EXECUTABLES_DIR) && timeout 15 mpirun -np 4 ./mpi_part_c_tags

clean:
	rm -f $(PART_A_EXE) $(PART_B_EXE) $(PART_C_EXE)
