# Installing Microsoft MPI on Windows

## Steps to Install:

1. **Download Microsoft MPI v10.1.3:**
   - Go to: https://www.microsoft.com/en-us/download/details.aspx?id=105289
   - Download both files:
     - `msmpisetup.exe` (7.4 MB) - Runtime
     - `msmpisdk.msi` (2.2 MB) - SDK for development

2. **Install Runtime:**
   - Run `msmpisetup.exe` as Administrator
   - Follow the installation wizard

3. **Install SDK:**
   - Run `msmpisdk.msi` as Administrator
   - This provides the `mpicc` compiler and headers

4. **Verify Installation:**
   ```cmd
   mpiexec --version
   ```

## Compilation Commands:

**For Example B (MPI):**
```cmd
mpicc example_b_mpi.c -o example_b_mpi.exe
```

**For Example C (Hybrid):**
```cmd
mpicc -fopenmp example_c_hybrid.c -o example_c_hybrid.exe
```

## Execution Commands:

**Run with 4 processes:**
```cmd
mpiexec -n 4 ./example_b_mpi.exe
mpiexec -n 4 ./example_c_hybrid.exe
``` 