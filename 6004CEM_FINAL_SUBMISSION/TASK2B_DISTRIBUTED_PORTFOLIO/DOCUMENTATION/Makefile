# 6004CEM TASK2B Distributed Portfolio Makefile (C++ Version)
MPICXX = mpicxx
CXXFLAGS = -O3 -Wall -Wextra -std=c++11
PROGRAMS_DIR = ../PROGRAMS
EXECUTABLES_DIR = ../EXECUTABLES

PART_A_SRC = $(PROGRAMS_DIR)/part_a_mpi_hello_world_enhanced.cpp
PART_B_SRC = $(PROGRAMS_DIR)/part_b_mpi_master_slave_enhanced.cpp
PART_C_SRC = $(PROGRAMS_DIR)/part_c_mpi_message_tags_enhanced.cpp

PART_A_EXE = $(EXECUTABLES_DIR)/part_a_mpi_hello_world_enhanced
PART_B_EXE = $(EXECUTABLES_DIR)/part_b_mpi_master_slave_enhanced
PART_C_EXE = $(EXECUTABLES_DIR)/part_c_mpi_message_tags_enhanced

.PHONY: all mpi clean test-mpi

all: mpi

mpi: $(PART_A_EXE) $(PART_B_EXE) $(PART_C_EXE)

$(PART_A_EXE): $(PART_A_SRC)
	@echo "🔨 Compiling Part A: MPI Hello World Enhanced (C++)..."
	$(MPICXX) $(CXXFLAGS) -o $@ $<

$(PART_B_EXE): $(PART_B_SRC)
	@echo "🔨 Compiling Part B: MPI Master-Slave Enhanced (C++)..."
	$(MPICXX) $(CXXFLAGS) -o $@ $<

$(PART_C_EXE): $(PART_C_SRC)
	@echo "🔨 Compiling Part C: MPI Message Tags Enhanced (C++)..."
	$(MPICXX) $(CXXFLAGS) -o $@ $<

test-mpi: mpi
	@echo "🧪 Testing TASK2B programs (C++)..."
	cd $(EXECUTABLES_DIR) && echo "4" | mpirun -np 4 ./part_a_mpi_hello_world_enhanced
	cd $(EXECUTABLES_DIR) && mpirun -np 4 ./part_b_mpi_master_slave_enhanced
	cd $(EXECUTABLES_DIR) && timeout 15 mpirun -np 4 ./part_c_mpi_message_tags_enhanced

clean:
	rm -f $(PART_A_EXE) $(PART_B_EXE) $(PART_C_EXE)
