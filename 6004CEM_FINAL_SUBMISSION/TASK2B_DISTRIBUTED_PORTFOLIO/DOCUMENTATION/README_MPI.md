# MPI Distributed Computing Components

## 6004CEM Parallel and Distributed Programming Coursework

This document provides comprehensive documentation for the MPI (Message Passing Interface) distributed computing components of the coursework.

---

## 📋 Overview

The distributed computing portfolio consists of three enhanced MPI programs that demonstrate various aspects of distributed parallel programming:

- **Part A (5%)**: Enhanced MPI Hello World with process mapping analysis
- **Part B (25%)**: Master-slave communication with structured messaging
- **Part C (20%)**: Message tagging demonstration with hang analysis

---

## 🔧 Prerequisites

### Required Software

- **MPI Implementation**: OpenMPI, MPICH, or Intel MPI
- **C Compiler**: GCC with C99 support
- **Development Tools**: Make, basic UNIX utilities

### Installation Commands

#### Ubuntu/Debian

```bash
sudo apt update
sudo apt install openmpi-bin openmpi-dev libopenmpi-dev
sudo apt install gcc make
```

#### CentOS/RHEL

```bash
sudo yum install openmpi openmpi-devel
sudo yum install gcc make
module load mpi  # If using environment modules
```

#### Verification

```bash
mpicc --version
mpirun --version
which mpicc mpirun
```

---

## 🏗️ Compilation

### Quick Start

```bash
# Build all MPI programs
make mpi

# Or build individual programs
make part_a_mpi_hello_world_enhanced
make part_b_mpi_master_slave_enhanced
make mpi_part_c_tags
```

### Manual Compilation

```bash
# Part A
mpicc -O2 -Wall -o part_a_mpi_hello_world_enhanced part_a_mpi_hello_world_enhanced.c -lm

# Part B
mpicc -O2 -Wall -o part_b_mpi_master_slave_enhanced part_b_mpi_master_slave_enhanced.c -lm

# Part C
mpicc -O2 -Wall -o mpi_part_c_tags mpi_part_c_tags.c -lm
```

---

## 🚀 Execution

### Part A: Enhanced MPI Hello World

**Basic Execution:**

```bash
# Run with 4 processes (as required)
mpirun -np 4 ./part_a_mpi_hello_world_enhanced

# Run with different process counts
mpirun -np 8 ./part_a_mpi_hello_world_enhanced
mpirun -np 2 ./part_a_mpi_hello_world_enhanced
```

**Expected Output:**

- System verification information
- Process mapping to physical cores
- Hello world from each MPI process
- Advanced MPI feature demonstrations
- User input for process count specification

**Key Features:**

- ✅ Process-to-core mapping analysis
- ✅ System information display for verification
- ✅ User input for dynamic process specification
- ✅ Performance timing measurements
- ✅ Advanced MPI communication examples

### Part B: Master-Slave Communication

**Basic Execution:**

```bash
# Run with 4 processes (1 master + 3 slaves)
mpirun -np 4 ./part_b_mpi_master_slave_enhanced

# Test with different numbers of slaves
mpirun -np 6 ./part_b_mpi_master_slave_enhanced  # 1 master + 5 slaves
mpirun -np 2 ./part_b_mpi_master_slave_enhanced  # 1 master + 1 slave
```

**Expected Output:**

- Basic master-slave communication (Part B.a)
- Personalized slave messages (Part B.b)
- Enhanced structured communication
- Asynchronous communication patterns
- Work coordination examples
- Performance analysis

**Key Features:**

- ✅ No slave processes print directly
- ✅ Master receives and displays all messages
- ✅ Each slave sends different personalized messages
- ✅ Advanced communication patterns
- ✅ Work distribution and coordination

### Part C: Message Tagging Demonstration

**Basic Execution:**

```bash
# Run tag demonstration
mpirun -np 4 ./mpi_part_c_tags
```

**Expected Behavior:**

1. **Scenario 1**: Working communication (tag 100 ↔ 100) ✅
2. **Scenario 2**: Hanging communication (tag 100 → 101) ⚠️ (with timeout protection)

**Key Features:**

- ✅ Demonstrates working tag matching
- ✅ Shows hang behavior with tag mismatch
- ✅ Comprehensive explanation of why hangs occur
- ✅ Timeout protection prevents infinite hangs
- ✅ Advanced tag usage patterns

---

## 🖥️ Cluster Execution

### Single Node (Shared Memory)

```bash
# Standard execution on single machine
mpirun -np 4 ./program_name

# Bind processes to specific cores
mpirun -np 4 --bind-to core ./program_name

# Display process placement
mpirun -np 4 --report-bindings ./program_name
```

### Multiple Nodes (Distributed Memory)

**Create hostfile:**

```bash
# hosts.txt
node1 slots=4
node2 slots=4
node3 slots=2
```

**Execute across nodes:**

```bash
# Run across multiple nodes
mpirun -np 8 --hostfile hosts.txt ./program_name

# Specify processes per node
mpirun -np 8 --hostfile hosts.txt --map-by ppr:2:node ./program_name
```

### Cluster-Specific Examples

**SLURM (Common in HPC environments):**

```bash
#!/bin/bash
#SBATCH --job-name=mpi_coursework
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4
#SBATCH --time=00:10:00

module load openmpi
mpirun ./part_a_mpi_hello_world_enhanced
```

**PBS/Torque:**

```bash
#!/bin/bash
#PBS -N mpi_coursework
#PBS -l nodes=2:ppn=4
#PBS -l walltime=00:10:00

cd $PBS_O_WORKDIR
mpirun -np 8 ./part_b_mpi_master_slave_enhanced
```

---

## 🧪 Testing and Validation

### Automated Testing

```bash
# Test all MPI programs
make test-mpi

# Individual tests
make run-mpi-a
make run-mpi-b
make run-mpi-c
```

### Manual Verification Steps

1. **Screenshot Requirements**:

   - Run programs on cluster with your username visible
   - Capture output showing system verification information
   - Demonstrate successful execution

2. **Functionality Checks**:

   ```bash
   # Verify Part A requirements
   echo "4" | mpirun -np 4 ./part_a_mpi_hello_world_enhanced | grep "Process.*Hello"

   # Verify Part B master-slave pattern
   mpirun -np 4 ./part_b_mpi_master_slave_enhanced | grep -c "Message received from process"

   # Verify Part C tag scenarios
   timeout 10 mpirun -np 4 ./mpi_part_c_tags
   ```

---

## 🔍 Troubleshooting

### Common Issues

#### MPI Not Found

```bash
# Symptoms: "mpicc: command not found"
# Solution: Install MPI or load module
module load mpi        # HPC clusters
export PATH=$PATH:/usr/lib64/openmpi/bin  # Manual path
```

#### Process Binding Errors

```bash
# Symptoms: Warning about process binding
# Solution: Use --oversubscribe flag
mpirun --oversubscribe -np 8 ./program_name
```

#### Hanging Programs

```bash
# If Part C hangs unexpectedly:
# Use timeout command
timeout 30 mpirun -np 4 ./mpi_part_c_tags

# Force kill if needed
pkill -f mpirun
```

#### Permission Denied on Cluster

```bash
# Ensure execute permissions
chmod +x part_a_mpi_hello_world_enhanced
chmod +x part_b_mpi_master_slave_enhanced
chmod +x mpi_part_c_tags
```

### Debug Mode

```bash
# Enable MPI debugging
mpirun --debug-daemons -np 4 ./program_name

# Verbose output
mpirun --verbose -np 4 ./program_name

# Check MPI environment
mpirun -np 1 env | grep -i mpi
```

---

## 📊 Performance Analysis

### Expected Performance Characteristics

**Part A**: Process creation and communication overhead

- 2 processes: ~0.001-0.005 seconds
- 4 processes: ~0.002-0.008 seconds
- 8 processes: ~0.005-0.015 seconds

**Part B**: Master-slave communication scaling

- Linear increase with number of slaves
- Communication overhead: O(n) where n = number of slaves

**Part C**: Tag processing overhead

- Working tags: Minimal overhead
- Mismatched tags: Immediate hang (with timeout protection)

### Measurement Tools

```bash
# Time execution
time mpirun -np 4 ./program_name

# Profile with MPE (if available)
mpirun -np 4 -mpe=mpilog ./program_name

# Memory usage
valgrind --tool=massif mpirun -np 2 ./program_name
```

---

## 📝 Assignment Requirements Checklist

### Part A (5% of portfolio)

- ✅ Hello world program using MPI
- ✅ Runs with 4 processes
- ✅ Comments on MPI process mapping to processors/cores
- ✅ Enhanced version with user input for process count
- ✅ Screenshot of cluster execution required

### Part B (25% of portfolio)

- ✅ Slave processes don't issue print statements
- ✅ Slaves send messages back to master with their rank
- ✅ Master receives and prints messages from slaves
- ✅ Each process sends different personalized message
- ✅ All print statements issued by master only

### Part C (20% of portfolio)

- ✅ Master sends messages with tag 100, slaves wait for tag 100 (works)
- ✅ Master sends messages with tag 100, slaves wait for tag 101 (hangs)
- ✅ Explanation provided for why program hangs
- ✅ Screenshot of cluster execution required

### Enhanced Features

- ✅ Comprehensive system verification
- ✅ Performance timing and analysis
- ✅ Advanced MPI communication patterns
- ✅ Error handling and timeout protection
- ✅ Detailed documentation and comments

---

## 📚 Additional Resources

### MPI Documentation

- [MPI Standard](https://www.mpi-forum.org/docs/)
- [OpenMPI Documentation](https://www.open-mpi.org/doc/)
- [MPICH User Guide](https://www.mpich.org/documentation/guides/)

### Learning Materials

- [MPI Tutorial](https://mpitutorial.com/)
- [HPC Carpentry MPI Lesson](https://hpc-carpentry.github.io/hpc-parallel-novice/)
- [Lawrence Livermore MPI Tutorial](https://hpc.llnl.gov/documentation/tutorials/mpi-performance-topics)

### Debugging Tools

- [Intel MPI Debug](https://software.intel.com/content/www/us/en/develop/documentation/mpi-developer-guide-linux/)
- [TotalView Debugger](https://totalview.io/)
- [GNU GDB with MPI](https://www.gnu.org/software/gdb/)

---

## 📧 Support

For technical issues or questions about the MPI components:

1. Check this documentation first
2. Verify MPI installation and environment
3. Test with simple examples before complex programs
4. Consult official MPI documentation
5. Contact course instructors with specific error messages

---

_Last Updated: 30/06/2025_
_Author: Matthew Loh Yet Marn_
_Course: 6004CEM Parallel and Distributed Programming_
